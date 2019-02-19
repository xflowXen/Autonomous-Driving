#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in; //object state
  P_ = P_in; //object covariance matrix
  F_ = F_in; //state transition matrix
  H_ = H_in; 
  R_ = R_in;
  Q_ = Q_in; //process covariance matrix
}

void KalmanFilter::Predict() {
  x_ = F_ * x_;
  MatrixXd F_prime = F_.transpose();
  P_ = F_ * P_ * F_prime + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  VectorXd y = z - H_*x_;
  MatrixXd H_prime = H_.transpose();
  MatrixXd S = H_*P_*H_prime + R_;
  MatrixXd Sinv = S.inverse();
  MatrixXd K =  P_*H_prime*Sinv;

  //new state estimate
  x_ = x_ + ( K*y );
  MatrixXd I = MatrixXd::Identity(4, 4);
  P_ = ( I - K*H_ )*P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  double px = x_[0];
  double py = x_[1];
  double vx = x_[2];
  double vy = x_[3];

  // skip zeros
  if( px == 0.00 && py == 0.00 ) {
    return;
  }
  
  VectorXd x_prime(3);
  double rho = sqrt( px*px + py*py );
  x_prime << rho, atan2( py, px ), ( px*vx + py*vy )/rho;
  VectorXd y = z - x_prime;
  if( y[1] > 3.1415926535 )
    y[1] -= 2.f*3.1415926535;
  if( y[1] < -3.1415926535 )
    y[1] += 2.f*3.1415926535;
  MatrixXd H_prime = H_.transpose();
  MatrixXd S = H_*P_*H_prime + R_;
  MatrixXd Sinv = S.inverse();
  MatrixXd K =  P_*H_prime*Sinv;

  // Compute new state
  x_ = x_ + ( K*y );
  MatrixXd I = MatrixXd::Identity(4, 4);
  P_ = ( I - K*H_ )*P_;

}
