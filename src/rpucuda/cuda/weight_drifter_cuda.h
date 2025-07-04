/**
 * (C) Copyright 2020, 2021, 2022, 2023, 2024 IBM. All Rights Reserved.
 *
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#pragma once

#include "cuda_util.h"
#include "weight_drifter.h"

namespace RPU {

template <typename T> class WeightDrifterCuda {

public:
  explicit WeightDrifterCuda(CudaContextPtr context, int size);
  explicit WeightDrifterCuda(CudaContextPtr, const WeightDrifter<T> &wd, int x_size, int d_size);
  WeightDrifterCuda() {};
  virtual ~WeightDrifterCuda() = default;

  WeightDrifterCuda(const WeightDrifterCuda<T> &); // = default;
  WeightDrifterCuda<T> &operator=(const WeightDrifterCuda<T> &) = default;
  WeightDrifterCuda(WeightDrifterCuda<T> &&) = default;
  WeightDrifterCuda<T> &operator=(WeightDrifterCuda<T> &&) = default;

  void apply(T *weights, T time_since_last_call);

  inline bool isActive() { return active_; };

  void saturate(T *weights, param_t *dev_4params);
  const T *getNu() const { return dev_nu_ == nullptr ? nullptr : dev_nu_->getDataConst(); };

  void dumpExtra(RPU::state_t &extra, const std::string prefix);
  void loadExtra(const RPU::state_t &extra, const std::string prefix, bool strict);

protected:
  CudaContextPtr context_ = nullptr;
  int size_ = 0;
  int max_size_ = 0;
  bool active_ = false;
  T current_t_ = 0.0;

  DriftParameter<T> par_;

  std::unique_ptr<CudaArray<T>> dev_previous_weights_ = nullptr;
  std::unique_ptr<CudaArray<T>> dev_w0_ = nullptr;
  std::unique_ptr<CudaArray<T>> dev_t_ = nullptr;
  std::unique_ptr<CudaArray<T>> dev_nu_ = nullptr;

private:
  void initialize(const T *weights);
  void
  populateFrom(const WeightDrifter<T> &wd, int x_size, int d_size); // called during construction
};

} // namespace RPU
