#include "glade.h"
#include "weather.h"

#include <QKeyEvent>
#include <QThread>

// display settings
bool wireframe = false;
double hangle = 0;
double vangle = 0;
int timeout = 20; 	// time to wait between rounds

double mouseX = -1000;
double mouseY = -10000;
/// weather settings
// the temperature is calculated as a simple sinus of time, with an offest of avgTemperature
// along the Y axix, and an amplitude of deltaTemp. it should nore or less align with summer and
// winter. temperature settings are in degrees.

/// time settings
// the smallest time unit is 1h
bool paused = false;

// used to check which way the viewer is going
int vMove = 0;
int hMove = 0;

hormones horms(6, 0, 0);

Glade::Glade(QWidget *parent) : x(0), y(20), z(5), QGLWidget(parent){
    t = newTree(TreeParams());
}

tree* Glade::newTree(TreeParams params){
    QThread* thread = new QThread;
    tree* t = new tree(params);
    t->moveToThread(thread);
    connect(thread, SIGNAL(started()), t, SLOT(init()));
    connect(this, SIGNAL(initializeGL()), t, SLOT(initGraphics()));
    connect(this, SIGNAL(paintGL()), t, SLOT(draw()));
    connect(this, SIGNAL(transport(int)), t, SLOT(transport(int)));
    connect(t, SIGNAL(finished()), thread, SLOT(quit()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(this, SIGNAL(draw(bool)), t, SLOT(draw(bool)));
    thread->start();
    return t;
}

Glade::~Glade(){
    if(t)
        delete t;
}

//QSize Glade::sizeHint() const{
//    return QSize(450, 450);
//}

QSize Glade::minimumSizeHint() const{
    return QSize(200, 200);
}

void Glade::initializeGL()    {
    GLenum err = glewInit();
    if (GLEW_OK != err){
      /* Problem: glewInit failed, something is seriously wrong. */
      std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }else{
        std::cout << "glew initialised" << std::endl;
    }
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glClearColor(135.0/255.0, 206.0/255.0, 250.0/255.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_COLOR_MATERIAL);
    std::cout << "Initalised colours\n";

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

//    t->initGraphics();
}

void Glade::paintGL(){
    glMatrixMode(GL_MODELVIEW);

    int n = 500;
    int m = 500;
//    float n2 = -(n/2.0);
//    float m2 = -(m/2.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat mat_amb_diff[] = { 0,1.0,0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                        mat_amb_diff);

    glDisable(GL_TEXTURE_2D);
    glBegin (GL_QUADS);
        if(Weather::getTemp() > 0)
            glColor4f(0,0.5,0,1);
        else
            glColor4f(1.0,1.0,1.0,1);
        glNormal3f(0.0,0.0,1.0);
        glVertex3f (-n, -n, 0);
        glVertex3f (-n, m, 0);
        glVertex3f (n, m, 0.0);
        glVertex3f (n, -n, 0.0);
    glEnd();

    t->draw();

    glPopMatrix();
//    std::cout << "printing" << std::endl;
}

void Glade::resizeGL(int width, int height){

        if(height == 0)
            height = 1;

        float ratio = 1.0 * width / height;

        /* Reset the coordinate system before modifying*/
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /* Set the viewport to be the entire window*/
            glViewport(0, 0, width, height);

        /* Set the correct perspective.*/
        gluPerspective(45,ratio,1,1000);
        resetViewPoint();
        glRotated(vangle,1,0,0);
        glRotated(hangle,0,0,-1);
        setlight();

}

void Glade::setlight(){
    GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 0.4 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    GLfloat light_position[] = {0.0,0.0,15,1.0};
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 0.1};
    GLfloat light_specular[] = { 0.2, 0.2, 0.2, 0.0 };
    GLfloat light_diffuse[] = { 0.2,0.2, 0.2, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    GLfloat light1_specular[] = { 0.9, 0.9, 0.9, 1.0 };
    GLfloat light1_position[] = { 4, 4, 9, 1.0 };
    GLfloat spot_direction[] = { 0, 0, -1};


    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 6.0);

    glEnable(GL_LIGHT1);
    glPopMatrix();
    glEnable(GL_LIGHT0);

    std::cout << "Initalised lights\n";

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {50.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void Glade::keyPressEvent(QKeyEvent *event){
        switch(event->key()){
       /* case Qt::Key_W: std::cout << event->key() << "\n";
            if(wireframe){
                  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                  wireframe = false;
            }else{
                  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                  wireframe = true;
            }
            break;
         */
        case Qt::Key_Q: exit(0); break;
//        case Qt::Key_A: horms.auxin += 0.1; t.effect(horms); break;
        case Qt::Key_W: vMove = 1; break;
        case Qt::Key_S: vMove = -1; break;
        case Qt::Key_A: hMove = 1; break;
        case Qt::Key_D: hMove = -1; break;
        case Qt::Key_Z: horms.auxin -= 0.1; t->effect(horms); break;
        case Qt::Key_H: //showHelp();
        case Qt::Key_P: paused = !paused;
//              glLogger::println("paused");
              break;
        case Qt::Key_R: y = 20; x = vangle = hangle = 0; z = 5; break;
        case Qt::Key_N: resetTree(0); break;
        case Qt::Key_Escape: setMouseTracking(false);

    }
    std::cout << x << " " << z << std::endl;
    resetViewPoint();
    update();
}

void Glade::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_W:
        case Qt::Key_S: vMove = 0; break;
        case Qt::Key_A:
        case Qt::Key_D: hMove = 0; break;
    }
}

void Glade::mouseReleaseEvent(QMouseEvent* event){
     setMouseTracking(true);
}

void Glade::mouseMoveEvent(QMouseEvent* event){
    resetViewPoint();
    if(mouseX > 0 && mouseY > 0){
        vangle += ((double)(event->globalY()-mouseY))/2;
        hangle += ((double)(event->globalX()-mouseX))/2;
        hangle = fmod(hangle,360);
        update();
        emit angleChanged(hangle, vangle);
    }
    mouseX = event->globalX();
    mouseY = event->globalY();
}

void Glade::wheelEvent(QWheelEvent *event){
    y -= event->delta()/50;
    resetViewPoint();
}

/*
 * z axis -> left-right
 * y axis -> tilt
 */
void Glade::resetViewPoint(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(vangle,1,0,0);
    glRotated(hangle,0,1,0);
    gluLookAt(x,y,z, 		// punkt z ktorego sie patrzy
              x,-1000000,z,		// punkt (dowolny) lezacy na linie wzroku
              0.0f,0.0f,1.0f);	// wektor ktory wskazuje gore

//    glRotated(vangle,0,0,1);
//    glRotated(vangle,0,0,1);
//    glRotated(hangle,1,0,0);
//    glTranslated(x, y, z);


}

void Glade::tick(){
    const double toRads = 3.14d/180.0;

//    for(int i = 0; i < Weather::getTimeStep(); i++){
//        t.effect(horms);
//        t.transport();
//    }
    emit transport(Weather::getTimeStep());

    double baseMove = 0.5;

    if(hMove != 0 || vMove != 0){
        double deltaZ = baseMove * sin(vangle *toRads);
        double horMove = baseMove * cos(vangle * toRads);
        double hSin = horMove * sin(hangle * toRads);
        double hCos = horMove * cos(hangle * toRads);

        if(vMove != 0){
            x += (vMove > 0 ? -hSin : hSin);
            y += (vMove > 0) ? -hCos : hCos;
            z += (vMove > 0 ? -deltaZ : deltaZ);
       }

        if(hMove != 0 ){
            x += (hMove < 0 ? -hCos : hCos);
            y += (hMove > 0 ? -hSin : hSin);
            z += (hMove < 0 ? -deltaZ : deltaZ);
        }
        emit coordsChanged(x, y, z);
    }


    update();
}

void Glade::setTreeParams(int tree, TreeParams params){
    t->setParams(params);
}

TreeParams Glade::getTreeParams(int tree){
    return t->getParams();
}

void Glade::setTreeHorms(int tree, hormones horms){
    t->effect(horms);
}

hormones Glade::getTreeHorms(int tree){
    return t->getHormones();
}

void Glade::resetTree(){
    resetTree(0);
}

void Glade::resetTree(int treeNo){
    TreeParams params = t->getParams();
    if(t){
        delete t;
    }
    t = newTree(params);
    t->initGraphics();
}


