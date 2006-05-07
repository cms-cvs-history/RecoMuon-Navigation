#ifndef MUON_TK_NAVIGATION_SCHOOL_H
#define MUON_TK_NAVIGATION_SCHOOL_H

#include "TrackingTools/DetLayers/interface/NavigationSchool.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "RecoMuon/Navigation/interface/MuonLayerSort.h"

class DetLayer;
class BarrelDetLayer;
class ForwardDetLayer;
class SymmetricLayerFinder;
class NavigableLayer;
class SimpleBarrelNavigableLayer;
class SimpleForwardNavigableLayer;
class MuonBarrelNavigableLayer;
class MuonForwardNavigableLayer;
class MagneticField;

#include <vector>

class MuonTkNavigationSchool : public NavigationSchool {

  public:

    /// Constructor
    MuonTkNavigationSchool(const MuonDetLayerGeometry*, const GeometricSearchTracker*, const MagneticField*);
   /// Destructor
   ~MuonTkNavigationSchool();
    /// return a vector of NavigableLayer*, from base class
    virtual vector<NavigableLayer*> navigableLayers() const;

  private:

    void addBarrelLayer(BarrelDetLayer*);
    /// add endcap layer (backward and forward)
    void addEndcapLayer(ForwardDetLayer*);
    /// link barrel layers
    void linkBarrelLayers();
    /// link endcap layers
    void linkEndcapLayers(const MapE&, 
                          std::vector<MuonForwardNavigableLayer*>&,
                          std::vector<SimpleForwardNavigableLayer*>&);

    float barrelLength();

    void createInverseLinks() const;

    float calculateEta(float r, float z) const;

  private:
  typedef vector<const DetLayer*>              DLC;
  typedef vector<BarrelDetLayer*>              BDLC;
  typedef vector<ForwardDetLayer*>             FDLC;
    typedef DLC::iterator                        DLI;
    typedef BDLC::iterator                       BDLI;
    typedef FDLC::iterator                       FDLI;
    typedef BDLC::const_iterator                 ConstBDLI;
    typedef FDLC::const_iterator                 ConstFDLI;

    MapB theBarrelLayers;
    MapE theForwardLayers;   // +z
    MapE theBackwardLayers;  // -z

    vector<SimpleBarrelNavigableLayer*>  theTkBarrelNLC;
    vector<SimpleForwardNavigableLayer*> theTkForwardNLC;
    vector<SimpleForwardNavigableLayer*> theTkBackwardNLC;

    vector<MuonBarrelNavigableLayer*> theMuonBarrelNLC;
    vector<MuonForwardNavigableLayer*> theMuonForwardNLC;
    vector<MuonForwardNavigableLayer*> theMuonBackwardNLC;

    const MuonDetLayerGeometry* theMuonDetLayerGeometry;
    const GeometricSearchTracker* theGeometricSearchTracker;
    const MagneticField* theMagneticField;
    float theBarrelLength;

};

#endif
