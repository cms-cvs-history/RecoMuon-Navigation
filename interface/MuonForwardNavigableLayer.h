#ifndef Navigation_MuonForwardNavigableLayer_H
#define Navigation_MuonForwardNavigableLayer_H

//   Ported from ORCA.
//   compatibleLayers(dir) and compatibleLayers(fts, dir) are added,
//   which return ALL DetLayers that are compatible with a given DetLayer.
//   $Date: 2006/03/22 01:22:27 $
//   $Revision: 1.1 $

/* Collaborating Class Declarations */
#include "RecoMuon/Navigation/interface/MuonLayerSort.h"
#include "RecoMuon/Navigation/interface/MuonEtaRange.h"

class DetLayer;
class ForwardDetLayer;

/* Base Class Headers */
#include "RecoMuon/Navigation/interface/MuonNavigableLayer.h"

/* C++ Headers */

/* ====================================================================== */

/* Class MuonForwardNavigableLayer Interface */

class MuonForwardNavigableLayer : public MuonNavigableLayer {

  public:

    /// Constructor 
//    MuonForwardNavigableLayer(ForwardDetLayer* fdl,
//                              const MapE& outerEndcap, 
//                              const MapE& innerEndcap) :
//      theDetLayer(fdl),
//      theOuterEndcapLayers(outerEndcap),
//      theInnerEndcapLayers(innerEndcap) {}

    MuonForwardNavigableLayer(ForwardDetLayer* fdl,
                              const MapE& outerEndcap,
                              const MapE& innerEndcap,
                              const MapE& allOuterEndcap,
                              const MapE& allInnerEndcap) :
      theDetLayer(fdl),
      theOuterEndcapLayers(outerEndcap),
      theInnerEndcapLayers(innerEndcap),
      theAllOuterEndcapLayers(allOuterEndcap),
      theAllInnerEndcapLayers(allInnerEndcap)  {}

    /// Constructor with outer layers only
    MuonForwardNavigableLayer(ForwardDetLayer* fdl,
                              const MapE& outerEndcap) :
      theDetLayer(fdl),
      theOuterEndcapLayers(outerEndcap) {}
    /// Constructor with all outer layers only
    MuonForwardNavigableLayer(ForwardDetLayer* fdl,
                              const MapE& outerEndcap, 
                              const MapE& allOuterEndcap) :
      theDetLayer(fdl),
      theOuterEndcapLayers(outerEndcap),
      theAllOuterEndcapLayers(allOuterEndcap) {}


    /// NavigableLayer interface
    virtual vector<const DetLayer*> nextLayers(PropagationDirection dir) const;

    /// NavigableLayer interface
    virtual vector<const DetLayer*> nextLayers(const FreeTrajectoryState& fts, 
                                               PropagationDirection dir) const;

    virtual vector<const DetLayer*> compatibleLayers(PropagationDirection dir) const;

    /// NavigableLayer interface
    virtual vector<const DetLayer*> compatibleLayers(const FreeTrajectoryState& fts,
                                               PropagationDirection dir) const;
    /// return DetLayer
    virtual DetLayer* detLayer() const;

    /// set DetLayer
    virtual void setDetLayer(DetLayer*);

    /// Operations
    MapE getOuterEndcapLayers() const { return theOuterEndcapLayers; }
    MapE getInnerEndcapLayers() const { return theInnerEndcapLayers; }
    MapB getInnerBarrelLayers() const { return theInnerBarrelLayers; }

    MapE getAllOuterEndcapLayers() const { return theAllOuterEndcapLayers; }
    MapE getAllInnerEndcapLayers() const { return theAllInnerEndcapLayers; }
    MapB getAllInnerBarrelLayers() const { return theAllInnerBarrelLayers; }

    /// set inward links
    void setInwardLinks(const MapB&, const MapE&);
    void setInwardCompatibleLinks(const MapB&, const MapE&);

  private:

    void pushResult(vector<const DetLayer*>& result, 
                    const MapB& map) const;

    void pushResult(vector<const DetLayer*>& result,
                     const MapE& map) const;

    void pushResult(vector<const DetLayer*>& result, 
                    const MapB& map, 
                    const FreeTrajectoryState& fts) const;

    void pushResult(vector<const DetLayer*>& result, 
                    const MapE& map, const
                    FreeTrajectoryState& fts) const;

    void pushCompatibleResult(vector<const DetLayer*>& result,
                    const MapB& map, const
                    FreeTrajectoryState& fts) const;

    void pushCompatibleResult(vector<const DetLayer*>& result,
                    const MapE& map, const
                    FreeTrajectoryState& fts) const;


  private:

    ForwardDetLayer* theDetLayer;
    MapE theOuterEndcapLayers;
    MapE theInnerEndcapLayers;
    MapB theInnerBarrelLayers;
    MapE theAllOuterEndcapLayers;
    MapE theAllInnerEndcapLayers;
    MapB theAllInnerBarrelLayers;


};
#endif
