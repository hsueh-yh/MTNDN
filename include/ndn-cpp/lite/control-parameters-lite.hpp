/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2015-2016 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with the additional exemption that
 * compiling, linking, and/or using OpenSSL is allowed.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * A copy of the GNU Lesser General Public License is in the file COPYING.
 */

#ifndef NDN_CONTROL_PARAMETERS_LITE_HPP
#define NDN_CONTROL_PARAMETERS_LITE_HPP

#include "common-lite.hpp"
#include "name-lite.hpp"
#include "forwarding-flags-lite.hpp"
#include "../c/control-parameters-types.h"

namespace ndn {

/**
 * A ControlParametersLite holds a Name and other fields for a
 * ControlParameters which is used, for example, in the command interest to
 * register a prefix with a forwarder.
 */
class ControlParametersLite : private ndn_ControlParameters {
public:
  /**
   * Create a ControlParametersLite to use the pre-allocated nameComponents and
   * strategyNameComponents, and with default field values.
   * @param nameComponents The pre-allocated array of ndn_NameComponent for the
   * main name. Instead of an array of NameLite::Component, this is an array of
   * the underlying ndn_NameComponent struct so that it doesn't run the default
   * constructor unnecessarily.
   * @param maxNameComponents The number of elements in the allocated
   * nameComponents array.
   * @param strategyNameComponents The pre-allocated array of ndn_NameComponent
   * for the strategy name. Instead of an array of NameLite::Component, this is
   * an array of the underlying ndn_NameComponent struct so that it doesn't run
   * the default constructor unnecessarily.
   * @param strategyMaxNameComponents The number of elements in the allocated
   * strategyNameComponents array.
   */
  ControlParametersLite
    (struct ndn_NameComponent *nameComponents, size_t maxNameComponents,
     struct ndn_NameComponent *strategyNameComponents,
     size_t strategyMaxNameComponents);

  /**
   * Check if the name is specified.
   * @return True if the name is specified, false if not.
   */
  bool
  getHasName() const { return hasName != 0; }

  /**
   * Get the name, if specified.
   * @return The Name. This is only meaningful if getHasName() is true.
   */
  NameLite&
  getName() { return NameLite::upCast(name); }

  const NameLite&
  getName() const { return NameLite::upCast(name); }

  int
  getFaceId() const { return faceId; }

  const BlobLite&
  getUri() const { return BlobLite::upCast(uri); }

  int
  getLocalControlFeature() const { return localControlFeature; }

  int
  getOrigin() const { return origin; }

  int
  getCost() const { return cost; }

  ForwardingFlagsLite&
  getForwardingFlags() { return ForwardingFlagsLite::upCast(flags); }

  const ForwardingFlagsLite&
  getForwardingFlags() const { return  ForwardingFlagsLite::upCast(flags); }

  NameLite&
  getStrategy() { return NameLite::upCast(strategy); }

  const NameLite&
  getStrategy() const { return NameLite::upCast(strategy); }

  Milliseconds
  getExpirationPeriod() const { return expirationPeriod; }

  /**
   * Set the flag for whether the name is specified. Note that setName
   * automatically calls setHasName(true).
   * @param hasName True if the name is specified, false if not.
   */
  void
  setHasName(bool hasName) { this->hasName = hasName ? 1 : 0; }

  /**
   * Set the name to have the values from the given name. This also calls
   * setHasName(true).
   * @param name The name to get values from. If the name is not specified, call
   * setHasName(false).
   * @return 0 for success, or an error code if there is not enough room in this
   * object's name components array.
   */
  ndn_Error
  setName(const NameLite& name)
  {
    hasName = 1;
    return NameLite::upCast(this->name).set(name);
  }

  /**
   * Set the Face ID.
   * @param faceId The new face ID, or -1 for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setFaceId(int faceId)
  {
    this->faceId = faceId;
    return *this;
  }

  /**
   * Set the URI.
   * @param uri The new uri, or an empty string for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setUri(const BlobLite& uri)
  {
    BlobLite::upCast(this->uri) = uri;
    return *this;
  }

  /**
   * Set the local control feature value.
   * @param localControlFeature The new local control feature value, or -1 for
   * not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setLocalControlFeature(int localControlFeature)
  {
    this->localControlFeature = localControlFeature;
    return *this;
  }

  /**
   * Set the origin value.
   * @param origin The new origin value, or -1 for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setOrigin(int origin)
  {
    this->origin = origin;
    return *this;
  }

  /**
   * Set the cost value.
   * @param cost The new cost value, or -1 for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setCost(int cost)
  {
    this->cost = cost;
    return *this;
  }

  /**
   * Set the ForwardingFlags object to a copy of forwardingFlags. You can use
   * getForwardingFlags() and change the existing ForwardingFlags object.
   * @param forwardingFlags The new cost value, or null for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setForwardingFlags(const ForwardingFlagsLite& flags)
  {
    ForwardingFlagsLite::upCast(this->flags) = flags;
    return *this;
  }

  /**
   * Set the strategy to a copy of the given Name.
   * @param strategy The Name to copy, or an empty Name if not specified.
   * @return 0 for success, or an error code if there is not enough room in this
   * object's name components array.
   */
  ndn_Error
  setStrategy(const NameLite& strategy)
  {
    return NameLite::upCast(this->strategy).set(strategy);
  }

  /**
   * Set the expiration period.
   * @param expirationPeriod The expiration period in milliseconds, or
   * null for not specified.
   * @return This ControlParametersLite so that you can chain calls to update
   * values.
   */
  ControlParametersLite&
  setExpirationPeriod(Milliseconds expirationPeriod)
  {
    this->expirationPeriod = expirationPeriod;
    return *this;
  }

  /**
   * Set this control parameters to have the values from the other control
   * parameters.
   * @param other The other ControlParametersLite to get values from.
   * @return 0 for success, or an error code if there is not enough room in this
   * object's components array.
   */
  ndn_Error
  set(const ControlParametersLite& other);

  /**
   * Upcast the reference to the ndn_ControlParameters struct to a
   * ControlParametersLite.
   * @param controlParameters A reference to the ndn_ControlParameters struct.
   * @return The same reference as ControlParametersLite.
   */
  static ControlParametersLite&
  upCast(ndn_ControlParameters& controlParameters)
  {
    return *(ControlParametersLite*)&controlParameters;
  }

  static const ControlParametersLite&
  upCast(const ndn_ControlParameters& controlParameters)
  {
    return *(ControlParametersLite*)&controlParameters;
  }

private:
  // Declare friends who can downcast to the private base.
  friend class Tlv0_1_1WireFormatLite;
};

}

#endif
