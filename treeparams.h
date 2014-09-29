#ifndef TREEPARAMS_H
#define TREEPARAMS_H
#include "leaf.h"
#include "parts.h"
#include "segment.h"
#include "bud.h"

class TreeParams{
    int budsnum; 		// number of buds per node
    float budAngle; 	// angle of rotation between nodes (in degrees)
    float branchAngle; 	// angle between child branch and main branch (in degrees)
    float segRad; 	// by how much should the radius of each brach grow
    float segLen; 		// by how much should each branch grow longer
    supplies baseSegmentUse;
    supplies segmentGrowthUse;
    hormones segmentHormones;
    hormones segmentHormonesProd;
    hormones segmentHormonesMin;
    int meristemTime; 	// the amount of time before a meristem grows a new segment

    int budGrowthTime; 	// the amount of time (in hours) before a bud is able to sprout
    supplies budGrowthUse;
    supplies budUse;
    hormones budHormonesProd;
    hormones budHormones;

    supplies leafUse; 	// amount of supplies used by each leaf
    supplies leafGrowth; 	// amount of supplies needed by each leaf to grow
    double leafSugarProduced;
    double leafGrowBy; 	// by how much (in %) a leaf grows
    double leafMaxSize; 	// max leaf size

public:
    TreeParams();
    void init();

    int getBudsnum(){return budsnum;}; 		// number of buds per node
    float getBudAngle(){return budAngle;}; 		// angle of rotation between nodes (in degrees)
    float getBranchAngle(){return branchAngle;}; 	// angle between child branch and main branch (in degrees)
    float getSegRad(){return segRad;}; 		// by how much should the radius of each brach grow
    float getSegLen(){return segLen;}; 		// by how much should each branch grow longer
    supplies getBaseSegmentUse(){return baseSegmentUse;};
    supplies getSegmentGrowthUse(){return segmentGrowthUse;};
    hormones getSegmentHormones(){return segmentHormones;}; 		// the optimal amount of various hormones for growth
    hormones getSegmentHormonesMin(){return segmentHormonesMin;}; 		// the min amount of various hormones for growth
    hormones getSegmentHormonesProd(){return segmentHormonesProd;}; 		// the influence of various hormones
    int getMeristemTime(){return meristemTime;}; 	// the amount of time (in hours) before a meristem produces a new section

    int getBudGrowthTime(){return budGrowthTime;}; 	// the amount of time (in hours) before a bud is able to sprout
    supplies getBudGrowthUse(){return budGrowthUse;}; 	// amount of supplies needed by a bud to sprout
    supplies getBudUse(){return budUse;}; 	// the amount of supplies used by a bud to survive
    hormones getBudHormonesProd(){return budHormonesProd;}; 		// how much auxin is produced by each bud
    hormones getBudHormones(){return budHormones;}; 		// the influence of various hormones

    supplies getLeafUse(){return leafUse;}; 	// amount of supplies used by each leaf
    supplies getLeafGrowth(){return leafGrowth;}; 	// amount of supplies needed by each leaf to grow
    double getLeafSugarProduced(){return leafSugarProduced;};// how much sugar is produced by a fully grown, healthy leaf
    double getLeafGrowBy(){return leafGrowBy;}; 	// by how much (in %) a leaf grows
    double getLeafMaxSize(){return leafMaxSize;}; 	// max size of a leaf

    void setBudsnum(int val){budsnum = val;}; 		// number of buds per node
    void setBudAngle(float val){budAngle = val;}; 		// angle of rotation between nodes (in degrees)
    void setBranchAngle(float val){branchAngle = val;}; 	// angle between child branch and main branch (in degrees)
    void setSegRad(float val){segRad = val;}; 		// by how much should the radius of each brach grow
    void setSegLen(float val){segLen = val;}; 		// by how much should each branch grow longer
    void setBaseSegmentUse(supplies val){baseSegmentUse = val;};
    void setSegmentGrowthUse(supplies val){segmentGrowthUse = val;};
    void setSegmentHormones(hormones val){segmentHormones = val;}; 		// the optimal amount of various hormones for growth
    void setSegmentHormonesMin(hormones val){segmentHormonesMin = val;}; 		// the min amount of various hormones for growth
    void setSegmentHormonesProd(hormones val){segmentHormonesProd = val;}; 		// the influence of various hormones
    void setMeristemTime(int val){meristemTime = val;}; 	// the amount of time (in hours) before a meristem produces a new section

    void setBudGrowthTime(int val){budGrowthTime = val;}; 	// the amount of time (in hours) before a bud is able to sprout
    void setBudGrowthUse(supplies val){budGrowthUse = val;}; 	// amount of supplies needed by a bud to sprout
    void setBudUse(supplies val){budUse = val;}; 	// the amount of supplies used by a bud to survive
    void setBudHormonesProd(hormones val){budHormonesProd = val;}; 		// how much auxin is produced by each bud
    void setBudHormones(hormones val){budHormones = val;}; 		// the influence of various hormones

    void setLeafUse(supplies val){leafUse = val;}; 	// amount of supplies used by each leaf
    void setLeafGrowth(supplies val){leafGrowth = val;}; 	// amount of supplies needed by each leaf to grow
    void setLeafSugarProduced(double val){leafSugarProduced = val;};// how much sugar is produced by a fully grown, healthy leaf
    void setLeafGrowBy(double val){leafGrowBy = val;}; 	// by how much (in %) a leaf grows
    void setLeafMaxSize(double val){leafMaxSize = val;}; 	// max size of a leaf
};

#endif // TREEPARAMS_H
