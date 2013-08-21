#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <assert.h>
#include <vector>

#include "Eigen/Dense"
class ForceVolume;
class Element
{
public:
  virtual ~Element();

  virtual std::vector<Eigen::Vector3f>
    GetNodalForces()=0;

  void addForce(ForceVolume * force){
    forces.push_back(force);
  }

  ///@param X and @param p in reference space
  virtual std::vector<float> InterpWeights(const Eigen::Vector3f & p,
      const std::vector<Eigen::Vector3f> & X)=0;

  virtual Eigen::Vector3f ShapeFunGrad(int ii,
       const Eigen::Vector3f & p,
       const std::vector<Eigen::Vector3f> & X)=0;
  ///@brief Computes deformation gradient at p
  virtual Eigen::Matrix3f GetDeformationGrad(const Eigen::Vector3f & p,
      const std::vector<Eigen::Vector3f> & X,
      const std::vector<Eigen::Vector3f> & u
      )=0;

  int GetNodeIndex(int elNodeIdx) const {
    assert(elNodeIdx >= 0 && elNodeIdx < nodeIndices.size());
    return nodeIndices[elNodeIdx];
  }

  std::vector<int> GetNodeIndices() const {
    return nodeIndices;
  }


  void SetIndices(const std::vector<int> & indices){
      nodeIndices = indices;
    }
private:
  std::vector<int> nodeIndices;
  std::vector<ForceVolume *> forces;

};
#endif
