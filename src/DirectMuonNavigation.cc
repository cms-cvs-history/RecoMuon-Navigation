#include "RecoMuon/Navigation/interface/DirectMuonNavigation.h"

/** \file DirectMuonNavigation
 *
 *  $Date: 2006/07/26 08:36:27 $
 *  $Revision: 1.3 $
 *  \author Chang Liu  -  Purdue University
 */

#include "TrackingTools/DetLayers/interface/BarrelDetLayer.h"
#include "TrackingTools/DetLayers/interface/ForwardDetLayer.h"
#include "TrackingTools/DetLayers/interface/NavigationSetter.h"
#include "Geometry/Surface/interface/BoundCylinder.h"
#include "Geometry/Surface/interface/BoundDisk.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "Utilities/General/interface/CMSexception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"

#include <algorithm>

using namespace std;

DirectMuonNavigation::DirectMuonNavigation(edm::ESHandle<MuonDetLayerGeometry> muonLayout) : theMuonDetLayerGeometry(muonLayout) {
   epsilon_ = 100.; 

}
/* Operations */ 
vector<const DetLayer*> 
DirectMuonNavigation::compatibleLayers( const FreeTrajectoryState& fts,
                                        PropagationDirection dir ) const {

  float z0 = fts.position().z();
  float zm = fts.momentum().z();
  float x0 = fts.position().x();

  bool inOut = outward(fts);

  vector<const DetLayer*> output;

  // check direction and position of FTS to get a correct order of DetLayers

  if (inOut) { 
     if ((zm * z0) >= 0) {
        inOutBarrel(fts,output);
        if ( z0 >= 0 ) inOutForward(fts,output);
        else inOutBackward(fts,output);
      } else {
        if ( z0 >= 0 ) outInForward(fts,output);
        else outInBackward(fts,output);
        inOutBarrel(fts,output);
      } 
   } else {
     if ((zm * z0) >= 0) {
        outInBarrel(fts,output);
        if ( z0 >= 0 ) inOutForward(fts,output);
        else inOutBackward(fts,output);
      } else {
        if ( z0 >= 0 ) outInForward(fts,output);
        else outInBackward(fts,output);
        outInBarrel(fts,output);
      } 
   }
// assume cosmic rays are coming from above. (true for most of them)
// so for top part, go outside in, for bottom part go inside out.
  if (x0 > 0) std::reverse(output.begin(),output.end());

  return output;
}


void DirectMuonNavigation::inOutBarrel(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {

  bool cont = false;
  vector<DetLayer*> barrel = theMuonDetLayerGeometry->allBarrelLayers();

  for (vector<DetLayer*>::const_iterator iter_B = barrel.begin(); iter_B != barrel.end(); iter_B++){

      if( cont ) output.push_back((*iter_B));
      else if ( checkCompatible(fts,dynamic_cast<const BarrelDetLayer*>(*iter_B))) {
      output.push_back((*iter_B));
      cont = true;
      }
  }
}


void DirectMuonNavigation::outInBarrel(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {

// default barrel layers are in out, reverse order 
  vector<DetLayer*> barrel = theMuonDetLayerGeometry->allBarrelLayers();

  bool cont = false;
  vector<DetLayer*>::const_iterator rbegin = barrel.end(); 
  rbegin--;
  vector<DetLayer*>::const_iterator rend = barrel.begin();
  rend--;

  for (vector<DetLayer*>::const_iterator iter_B = rbegin; iter_B != rend; iter_B--){
      if( cont ) output.push_back((*iter_B));
      else if ( checkCompatible(fts,dynamic_cast<BarrelDetLayer*>(*iter_B))) {
      output.push_back((*iter_B));
      cont = true;
      }
  }
}

void DirectMuonNavigation::inOutForward(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {

  vector<DetLayer*> forward = theMuonDetLayerGeometry->allForwardLayers();
  bool cont = false;
  for (vector<DetLayer*>::const_iterator iter_E = forward.begin(); iter_E != forward.end(); 
	 iter_E++){
      if( cont ) output.push_back((*iter_E));
      else if ( checkCompatible(fts,dynamic_cast<ForwardDetLayer*>(*iter_E))) {
	output.push_back((*iter_E));
	cont = true;
      }
    }
}

void DirectMuonNavigation::outInForward(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {
// default forward layers are in out, reverse order

  bool cont = false;
  vector<DetLayer*> forward = theMuonDetLayerGeometry->allForwardLayers();
  vector<DetLayer*>::const_iterator rbegin = forward.end();
  rbegin--;
  vector<DetLayer*>::const_iterator rend = forward.begin();
  rend--;
  for (vector<DetLayer*>::const_iterator iter_E = rbegin; iter_E != rend;
         iter_E--){
      if( cont ) output.push_back((*iter_E));
      else if ( checkCompatible(fts,dynamic_cast<ForwardDetLayer*>(*iter_E))) {
        output.push_back((*iter_E));
        cont = true;
      }
    }
}

void DirectMuonNavigation::inOutBackward(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {
  bool cont = false;
  vector<DetLayer*> backward = theMuonDetLayerGeometry->allBackwardLayers();

  for (vector<DetLayer*>::const_iterator iter_E = backward.begin(); iter_E != backward.end(); 
       iter_E++){
      if( cont ) output.push_back((*iter_E));
      else if ( checkCompatible(fts,dynamic_cast<ForwardDetLayer*>(*iter_E))) {
	output.push_back((*iter_E));
	cont = true;
      }
   }
}

void DirectMuonNavigation::outInBackward(const FreeTrajectoryState& fts, vector<const DetLayer*>& output) const {

  bool cont = false;
  vector<DetLayer*> backward = theMuonDetLayerGeometry->allBackwardLayers();

  vector<DetLayer*>::const_iterator rbegin = backward.end();
  rbegin--;
  vector<DetLayer*>::const_iterator rend = backward.begin();
  rend--;
  for (vector<DetLayer*>::const_iterator iter_E = rbegin; iter_E != rend;
       iter_E--){
      if( cont ) output.push_back((*iter_E));
      else if ( checkCompatible(fts,dynamic_cast<ForwardDetLayer*>(*iter_E))) {
        output.push_back((*iter_E));
        cont = true;
      }
   }

}


bool DirectMuonNavigation::checkCompatible(const FreeTrajectoryState& fts,const BarrelDetLayer* dl) const {

  float z0 = fts.position().z();
  float r0 = fts.position().perp();
  float zm = fts.momentum().z();
  float rm = fts.momentum().perp();
  float slope = zm/rm; 
  if (!outward(fts) ) slope = -slope;
  const BoundCylinder* bc = dynamic_cast<const BoundCylinder*>(&dl->surface());
  float radius = bc->radius();
  float length = bc->bounds().length()/2.;

  float z1 = slope*(radius - r0) + z0;
  return ( fabs(z1) <= fabs(length)+epsilon_ );

}

bool DirectMuonNavigation::checkCompatible(const FreeTrajectoryState& fts,const ForwardDetLayer* dl) const {

  float z0 = fts.position().z();
  float r0 = fts.position().perp();
  float zm = fts.momentum().z();
  float rm = fts.momentum().perp();
  float slope = rm/zm; 

  if (!outward(fts) ) slope = -slope;

  const BoundDisk* bd = dynamic_cast<const BoundDisk*>(&dl->surface());

  float outRadius = bd->outerRadius();
  float inRadius = bd->innerRadius();
  float z = bd->position().z();

  float r1 = slope*(z - z0) + r0;
  return (r1 >= inRadius-epsilon_ && r1 <= outRadius+epsilon_);

}

bool DirectMuonNavigation::outward(const FreeTrajectoryState& fts) const {
  float x0 = fts.position().x();
  float y0 = fts.position().y();
  float r0 = fts.position().perp();

  float xm = fts.momentum().x();
  float ym = fts.momentum().y();

  float delta = 0.01;

  float x1 = x0 + xm * delta;
  float y1 = y0 + ym * delta;

  float r1 = sqrt(x1*x1+y1*y1);

  return (r1 >= r0);
}