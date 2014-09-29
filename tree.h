#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <QObject>

#include "treeparams.h"
#include "leaf.h"
#include "parts.h"
#include "segment.h"
#include "bud.h"

class tree : public QObject {
    Q_OBJECT
private:
	part* trunk;
	supplies s;
	hormones h;
    TreeParams params;

    static int leafRenderRound; 	// round when leaves should be rendered
    GLuint leafTexture;
    GLuint barkTexture;

    static int renderRounds;
    treeShader::shader leafShader;

public slots:
    void init();
    void initGraphics();
    int grow();
    int transport(int times=1);
    double effect(hormones& h); // used to change this part with a hormone
    int winter();
    void draw(bool textured = true);

signals:
    void grown();

public:
	tree();
    tree(TreeParams param);
    ~tree();
    void setParams(TreeParams params);
    TreeParams getParams();
    void print();
    hormones getHormones(){return h;};

};
#endif // TREE_H
