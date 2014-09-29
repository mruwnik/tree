#ifndef GLADE_H
#define GLADE_H

#include <iostream>
#include <math.h>
#include "tree.h"
#include <QtOpenGL/QGLWidget>
#include <stdio.h>

class Glade : public QGLWidget
{
    Q_OBJECT

public:
    explicit Glade(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    void setTreeParams(int tree, TreeParams params);
    TreeParams getTreeParams(int tree);
    void setTreeHorms(int tree, hormones horms);
    hormones getTreeHorms(int tree);

    ~Glade();

public slots:
    void tick();
    void resetTree();
    void resetTree(int tree);

signals:
    void angleChanged(double hor, double vert);
    void coordsChanged(double x, double y, double z);
    void transport(int times=1);
    void draw(bool textured = true);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyReleaseEvent(QKeyEvent * event);
    void keyPressEvent(QKeyEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent * event);
    void setlight();
    void resetViewPoint();
    tree* newTree(TreeParams params);

private:
    double x,y,z;
    tree* t;
};

#endif // GLADE_H
