#include "treeparams.h"

TreeParams::TreeParams(){
    budsnum = 2; 		// number of buds per node
    budAngle = 0; 	// angle of rotation between nodes (in degrees)
    branchAngle = 30; 	// angle between child branch and main branch (in degrees)
    segRad = 0.000025; 	// by how much should the radius of each brach grow
    segLen = 0.005; 		// by how much should each branch grow longer
    baseSegmentUse = supplies(1,0,0);
    segmentGrowthUse = supplies(1,0,1);
    segmentHormones = hormones(8,4,0);
    segmentHormonesProd = hormones(0,0,0);
    segmentHormonesMin = hormones(0.2,0,0);
    meristemTime = 360; 	// the amount of time before a meristem grows a new segment

    budGrowthTime = 360; 	// the amount of time (in hours) before a bud is able to sprout
    budGrowthUse = supplies(1,2,1);
    budUse = supplies(1,0,0);
    budHormonesProd = hormones(0.5,0,0);
    budHormones = hormones(4,4,0);

    leafUse = supplies(4, 1, 0); 	// amount of supplies used by each leaf
    leafGrowth = supplies(1, 1, 1); 	// amount of supplies needed by each leaf to grow
    leafSugarProduced = 2.5;
    leafGrowBy = 1; 	// by how much (in %) a leaf grows
    leafMaxSize = 1;
}

void TreeParams::init(){
    double timedilation = 1;
    segment::setGrowth(segmentGrowthUse / timedilation);
    segment::setBudNum(budsnum);
    segment::setBudAngle(budAngle);
    segment::setBranchAngle(branchAngle);
    segment::setRate(segLen / timedilation, segRad / timedilation, meristemTime * timedilation);
    segment::setUses(baseSegmentUse / timedilation);
    segment::setHormoneProd(segmentHormonesProd / timedilation);
    segment::setHormoneEffect(segmentHormones / timedilation);
    segment::setHormoneMin(segmentHormonesMin);

    bud::setGrowthTime(budGrowthTime * timedilation);
    bud::setGrowth(budGrowthUse / timedilation);
    bud::setUse(budUse / timedilation);
    bud::setHormonesProd(budHormonesProd / timedilation);
    bud::setHormone(budHormones / timedilation);

    leaf::setUses(leafUse / timedilation);
    leaf::setGrowth(leafGrowth / timedilation);
    leaf::setSugarProduced(leafSugarProduced);
    leaf::setSize(leafMaxSize,leafGrowBy / timedilation);
}

