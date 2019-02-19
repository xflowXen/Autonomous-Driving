#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

/**
 * Constructor.
 */
FusionEKF::FusionEKF() {
  is_initialized_ = false;

  previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);
  x = VectorXd(4);
  P = MatrixXd(4,4);
  F = MatrixXd(4,4);
  Q = MatrixXd(4, 4);
  
  //empty covariance matrix - null
  R_null_ = MatrixXd(1,1);
  R_null_ << 0;

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
              0, 0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
              0, 0.0009, 0,
              0, 0, 0.09;
  //measurement matrix - laser
   H_laser_ << 1, 0, 0, 0,
			   0, 1, 0, 0;
  
  //initialising process and measurement noise
  x << 1,1,1,1;
  P << 1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 999, 0,
       0, 0, 0, 999;
  F << 1, 0, 1, 0,
       0, 1, 0, 1,
       0, 0, 1, 0,
       0, 0, 0, 1;
  
  stoch_ax = 9.00;
  stoch_ay = 9.00;
}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {
  /**
   * Initialization
   */
  if (!is_initialized_) {
    /**
     * TODO: Initialize the state ekf_.x_ with the first measurement.
     * TODO: Create the covariance matrix.
     * You'll need to convert radar from polar to cartesian coordinates.
     */
    //Initialise ekf
    ekf_.Init( x, P, F, Hj_, R_null_,Q );
    
    // first measurement
    cout << "EKF: initialised" << endl;
    //ekf_.x_ = VectorXd(4);
    //ekf_.x_ << x;
    
    

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
       double rho = measurement_pack.raw_measurements_[0];      
       double phi = measurement_pack.raw_measurements_[1];      
       double rho_rate = measurement_pack.raw_measurements_[2];
       ekf_.x_ << rho * cos(phi), rho * sin(phi),0, 0; //polar
       ekf_.R_ = R_radar_;

    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      ekf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0; // linear
      ekf_.R_ = R_laser_;
    }
    
    // capture timestamp for delta_t
    previous_timestamp_ = measurement_pack.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /**
   * Prediction
   */

  /**
   * TODO: Update the state transition matrix F according to the new elapsed time.
   * Time is measured in seconds.
   * TODO: Update the process noise covariance matrix.
   * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  double delta_t = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;  //secs
  previous_timestamp_ = measurement_pack.timestamp_;

  double delta_t2 = delta_t * delta_t;
  double delta_t3 = delta_t2 * delta_t;
  double delta_t4 = delta_t3 * delta_t;

  // Add time to state transition matrix
  cout << "here1" << endl;
  ekf_.F_(0, 2) = delta_t;
  ekf_.F_(1, 3) = delta_t;
  cout << "here2" << endl;

  //set the process covariance matrix Q
  ekf_.Q_ = Q;
  ekf_.Q_ << delta_t4/4*stoch_ax,   0,                    delta_t3/2*stoch_ax,  0,
             0,                     delta_t4/4*stoch_ay,  0,                    delta_t3/2*stoch_ay,
             delta_t3/2*stoch_ax,   0,                    delta_t2*stoch_ax,    0,
             0,                     delta_t3/2*stoch_ay,  0,                    delta_t2*stoch_ay;

  ekf_.Predict();

  /**
   * Update
   */

  /**
   * TODO:
   * - Use the sensor type to perform the update step.
   * - Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    Hj_ = tools.CalculateJacobian(ekf_.x_);
    ekf_.H_ = Hj_;
    ekf_.R_ = R_radar_;
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);

  } else {
    ekf_.H_ = H_laser_;
    ekf_.R_ = R_laser_;
    ekf_.Update(measurement_pack.raw_measurements_);

  }

  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}
