#include "tools.h"
#include <iostream>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  VectorXd rmse(4);
  rmse << 0,0,0,0;

  // sanity checks
  if(estimations.size() != ground_truth.size() || estimations.size() == 0){
    std::cout << "Invalid data detected... discarding..." << std::endl;
    return rmse;
  }

  for(unsigned int i=0; i < estimations.size(); i++){
    VectorXd diff = estimations[i] - ground_truth[i];
    diff = diff.array()*diff.array();
    rmse += diff;
  }
  rmse = rmse/estimations.size();
  rmse = rmse.array().sqrt();
  return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  //intialisations
  MatrixXd H_jacobian(3,4);
  H_jacobian << 0,0,0,0,
                0,0,0,0,
                0,0,0,0;
  double px = x_state(0);
  double py = x_state(1);
  double vx = x_state(2);
  double vy = x_state(3);

  //precalcs
  double pc1 = px*px+py*py;
  double pc2 = sqrt(pc1);
  double pc3 = (pc1*pc2);

  //compute Jacobian
  H_jacobian << (px/pc2),        (py/pc2),                0,      0,
        -(py/pc1),               (px/pc1),                0,      0, 
        py*(vx*py - vy*px)/pc3,  px*(px*vy - py*vx)/pc3,  px/pc2,  py/pc2;

  return H_jacobian;
}
