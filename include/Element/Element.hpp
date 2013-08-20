#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <assert.h>
#include <vector>

#include "Eigen/Dense"

class Element
{
public:
  virtual ~Element();

  ///@param X in reference space
  virtual std::vector<float> InterpWeights(const Eigen::Vector3f & X,
      const std::vector<Eigen::Vector3f> & nodes)=0;

  void SetIndices(const std::vector<int> & indices){
    nodeIndices = indices;
  }

  int GetNodeIndex(int elNodeIdx) const {
    assert(elNodeIdx >= 0 && elNodeIdx < nodeIndices.size());
    return nodeIndices[elNodeIdx];
  }

  std::vector<int> GetNodeIncides() const {
    return nodeIndices;
  }

private:
  std::vector<int> nodeIndices;
};
#endif
