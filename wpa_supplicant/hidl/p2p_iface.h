/*
 * hidl interface for wpa_supplicant daemon
 * Copyright (c) 2004-2016, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2016, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_SUPPLICANT_HIDL_P2P_IFACE_H
#define WPA_SUPPLICANT_HIDL_P2P_IFACE_H

#include <array>
#include <vector>

#include <android-base/macros.h>

#include <android/hardware/wifi/supplicant/1.0/ISupplicantP2pIface.h>
#include <android/hardware/wifi/supplicant/1.0/ISupplicantP2pIfaceCallback.h>
#include <android/hardware/wifi/supplicant/1.0/ISupplicantP2pNetwork.h>

extern "C" {
#include "utils/common.h"
#include "utils/includes.h"
#include "wpa_supplicant_i.h"
#include "config.h"
#include "driver_i.h"
}

namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
namespace V1_0 {
namespace implementation {

/**
 * Implementation of P2pIface hidl object. Each unique hidl
 * object is used for control operations on a specific interface
 * controlled by wpa_supplicant.
 */
class P2pIface
    : public android::hardware::wifi::supplicant::V1_0::ISupplicantP2pIface
{
public:
	P2pIface(struct wpa_global* wpa_global, const char ifname[]);
	~P2pIface() override = default;
	// Refer to |StaIface::invalidate()|.
	void invalidate();
	bool isValid();

	// Hidl methods exposed.
	Return<void> getName(getName_cb _hidl_cb) override;
	Return<void> getType(getType_cb _hidl_cb) override;
	Return<void> addNetwork(addNetwork_cb _hidl_cb) override;
	Return<void> removeNetwork(
	    SupplicantNetworkId id, removeNetwork_cb _hidl_cb) override;
	Return<void> getNetwork(
	    SupplicantNetworkId id, getNetwork_cb _hidl_cb) override;
	Return<void> listNetworks(listNetworks_cb _hidl_cb) override;
	Return<void> registerCallback(
	    const sp<ISupplicantP2pIfaceCallback>& callback,
	    registerCallback_cb _hidl_cb) override;
	Return<void> getDeviceAddress(getDeviceAddress_cb _hidl_cb) override;
	Return<void> setSsidPostfix(
	    const hidl_string& postfix, setSsidPostfix_cb _hidl_cb) override;
	Return<void> setGroupIdle(
	    uint32_t timeout_in_sec, setGroupIdle_cb _hidl_cb) override;
	Return<void> setPowerSave(
	    bool enable, setPowerSave_cb _hidl_cb) override;
	Return<void> find(uint32_t timeout_in_sec, find_cb _hidl_cb) override;
	Return<void> stopFind(stopFind_cb _hidl_cb) override;
	Return<void> flush(flush_cb _hidl_cb) override;
	Return<void> connect(
	    const hidl_array<uint8_t, 6>& peer_address,
	    ISupplicantP2pIface::WpsProvisionMethod provision_method,
	    const hidl_vec<uint8_t>& pre_selected_pin, bool join_existing_group,
	    bool persistent, uint32_t go_intent, connect_cb _hidl_cb) override;
	Return<void> cancelConnect(cancelConnect_cb _hidl_cb) override;
	Return<void> provisionDiscovery(
	    const hidl_array<uint8_t, 6>& peer_address,
	    ISupplicantP2pIface::WpsProvisionMethod provision_method,
	    provisionDiscovery_cb _hidl_cb) override;
	Return<void> addGroup(
	    bool persistent, uint32_t persistent_network_id,
	    addGroup_cb _hidl_cb) override;
	Return<void> removeGroup(
	    const hidl_string& group_ifname, removeGroup_cb _hidl_cb) override;
	Return<void> reject(
	    const hidl_array<uint8_t, 6>& peer_address,
	    reject_cb _hidl_cb) override;
	Return<void> invite(
	    const hidl_string& group_ifname,
	    const hidl_array<uint8_t, 6>& go_device_address,
	    const hidl_array<uint8_t, 6>& peer_address,
	    invite_cb _hidl_cb) override;
	Return<void> reinvoke(
	    uint32_t persistent_network_id,
	    const hidl_array<uint8_t, 6>& peer_address,
	    reinvoke_cb _hidl_cb) override;
	Return<void> configureExtListen(
	    bool enable, uint32_t period_in_millis, uint32_t interval_in_millis,
	    configureExtListen_cb _hidl_cb) override;
	Return<void> setListenChannel(
	    uint32_t channel, uint32_t operating_class,
	    setListenChannel_cb _hidl_cb) override;
	Return<void> getSsid(
	    const hidl_array<uint8_t, 6>& peer_address,
	    getSsid_cb _hidl_cb) override;
	Return<void> getGroupCapability(
	    const hidl_array<uint8_t, 6>& peer_address,
	    getGroupCapability_cb _hidl_cb) override;
	Return<void> addBonjourService(
	    const hidl_vec<uint8_t>& query, const hidl_vec<uint8_t>& response,
	    addBonjourService_cb _hidl_cb) override;
	Return<void> removeBonjourService(
	    const hidl_vec<uint8_t>& query,
	    removeBonjourService_cb _hidl_cb) override;
	Return<void> addUpnpService(
	    uint32_t version, const hidl_string& service_name,
	    addUpnpService_cb _hidl_cb) override;
	Return<void> removeUpnpService(
	    uint32_t version, const hidl_string& service_name,
	    removeUpnpService_cb _hidl_cb) override;
	Return<void> flushServices(
	    uint32_t version, const hidl_string& service_name,
	    flushServices_cb _hidl_cb) override;
	Return<void> requestServiceDiscovery(
	    const hidl_array<uint8_t, 6>& peer_address,
	    const hidl_vec<uint8_t>& query,
	    requestServiceDiscovery_cb _hidl_cb) override;
	Return<void> cancelServiceDiscovery(
	    uint64_t identifier, cancelServiceDiscovery_cb _hidl_cb) override;

private:
	// Corresponding worker functions for the HIDL methods.
	std::pair<SupplicantStatus, std::string> getNameInternal();
	std::pair<SupplicantStatus, IfaceType> getTypeInternal();
	std::pair<SupplicantStatus, sp<ISupplicantP2pNetwork>>
	addNetworkInternal();
	SupplicantStatus removeNetworkInternal(SupplicantNetworkId id);
	std::pair<SupplicantStatus, sp<ISupplicantP2pNetwork>>
	getNetworkInternal(SupplicantNetworkId id);
	std::pair<SupplicantStatus, std::vector<SupplicantNetworkId>>
	listNetworksInternal();
	SupplicantStatus registerCallbackInternal(
	    const sp<ISupplicantP2pIfaceCallback>& callback);
	std::pair<SupplicantStatus, std::array<uint8_t, 6>>
	getDeviceAddressInternal();
	SupplicantStatus setSsidPostfixInternal(const hidl_string& postfix);
	SupplicantStatus setGroupIdleInternal(uint32_t timeout_in_sec);
	SupplicantStatus setPowerSaveInternal(bool enable);
	SupplicantStatus findInternal(uint32_t timeout_in_sec);
	SupplicantStatus stopFindInternal();
	SupplicantStatus flushInternal();
	std::pair<SupplicantStatus, std::vector<uint8_t>> connectInternal(
	    const std::array<uint8_t, 6>& peer_address,
	    ISupplicantP2pIface::WpsProvisionMethod provision_method,
	    const std::vector<uint8_t>& pre_selected_pin,
	    bool join_existing_group, bool persistent, uint32_t go_intent);
	SupplicantStatus cancelConnectInternal();
	SupplicantStatus provisionDiscoveryInternal(
	    const std::array<uint8_t, 6>& peer_address,
	    ISupplicantP2pIface::WpsProvisionMethod provision_method);
	SupplicantStatus addGroupInternal(
	    bool persistent, uint32_t persistent_network_id);
	SupplicantStatus removeGroupInternal(const hidl_string& group_ifname);
	SupplicantStatus rejectInternal(
	    const std::array<uint8_t, 6>& peer_address);
	SupplicantStatus inviteInternal(
	    const hidl_string& group_ifname,
	    const std::array<uint8_t, 6>& go_device_address,
	    const std::array<uint8_t, 6>& peer_address);
	SupplicantStatus reinvokeInternal(
	    uint32_t persistent_network_id,
	    const std::array<uint8_t, 6>& peer_address);
	SupplicantStatus configureExtListenInternal(
	    bool enable, uint32_t period_in_millis,
	    uint32_t interval_in_millis);
	SupplicantStatus setListenChannelInternal(
	    uint32_t channel, uint32_t operating_class);
	std::pair<SupplicantStatus, std::vector<uint8_t>> getSsidInternal(
	    const std::array<uint8_t, 6>& peer_address);
	std::pair<SupplicantStatus, uint32_t> getGroupCapabilityInternal(
	    const std::array<uint8_t, 6>& peer_address);
	SupplicantStatus addBonjourServiceInternal(
	    const std::vector<uint8_t>& query,
	    const std::vector<uint8_t>& response);
	SupplicantStatus removeBonjourServiceInternal(
	    const std::vector<uint8_t>& query);
	SupplicantStatus addUpnpServiceInternal(
	    uint32_t version, const hidl_string& service_name);
	SupplicantStatus removeUpnpServiceInternal(
	    uint32_t version, const hidl_string& service_name);
	SupplicantStatus flushServicesInternal(
	    uint32_t version, const hidl_string& service_name);
	std::pair<SupplicantStatus, uint64_t> requestServiceDiscoveryInternal(
	    const std::array<uint8_t, 6>& peer_address,
	    const std::vector<uint8_t>& query);
	SupplicantStatus cancelServiceDiscoveryInternal(uint64_t identifier);

	struct wpa_supplicant* retrieveIfacePtr();

	// Reference to the global wpa_struct. This is assumed to be valid for
	// the lifetime of the process.
	const struct wpa_global* wpa_global_;
	// Name of the iface this hidl object controls
	const std::string ifname_;
	bool is_valid_;

	DISALLOW_COPY_AND_ASSIGN(P2pIface);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace wifi
}  // namespace supplicant
}  // namespace hardware
}  // namespace android

#endif  // WPA_SUPPLICANT_HIDL_P2P_IFACE_H
