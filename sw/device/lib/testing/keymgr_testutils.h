// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_LIB_TESTING_KEYMGR_TESTUTILS_H_
#define OPENTITAN_SW_DEVICE_LIB_TESTING_KEYMGR_TESTUTILS_H_

#include "sw/device/lib/dif/dif_keymgr.h"

/**
 * Versioned key parameters for testing.
 *
 * Change destination in order to sideload keys to hardware.
 */
static const dif_keymgr_versioned_key_params_t kKeyVersionedParams = {
    .dest = kDifKeymgrVersionedKeyDestSw,
    .salt =
        {
            0xb6521d8f,
            0x13a0e876,
            0x1ca1567b,
            0xb4fb0fdf,
            0x9f89bc56,
            0x4bd127c7,
            0x322288d8,
            0xde919d54,
        },
    .version = 0xaa,
};

/**
 * Software binding value for advancing to creator root key state.
 */
static const dif_keymgr_state_params_t kCreatorParams = {
    .binding_value = {0xdc96c23d, 0xaf36e268, 0xcb68ff71, 0xe92f76e2,
                      0xb8a8379d, 0x426dc745, 0x19f5cff7, 0x4ec9c6d6},
    .max_key_version = 0x11,
};

/**
 * Software binding value for advancing to owner intermediate key state.
 */
static const dif_keymgr_state_params_t kOwnerIntParams = {
    .binding_value = {0xe4987b39, 0x3f83d390, 0xc2f3bbaf, 0x3195dbfa,
                      0x23fb480c, 0xb012ae5e, 0xf1394d28, 0x1940ceeb},
    .max_key_version = 0xaa,
};

/**
 * Initialize flash for key manager operation.
 */
void keymgr_testutils_init_flash(void);

/**
 * Issues a keymgr advance operation and wait for it to complete
 *
 * @param keymgr A key manager handle.
 * @param params The binding and max key version value for the next state.
 */
void keymgr_testutils_advance_state(const dif_keymgr_t *keymgr,
                                    const dif_keymgr_state_params_t *params);

/**
 * Checks if the current keymgr state matches the expected state
 *
 * @param keymgr A key manager handle.
 * @param exp_state The expected key manager state.
 */
void keymgr_testutils_check_state(const dif_keymgr_t *keymgr,
                                  const dif_keymgr_state_t exp_state);

/**
 * Issues a keymgr identity generation and wait for it to complete
 *
 * @param keymgr A key manager handle.
 */
void keymgr_testutils_generate_identity(const dif_keymgr_t *keymgr);

/**
 * Issues a keymgr HW/SW versioned key generation and wait for it to complete
 *
 * @param keymgr A key manager handle.
 * @param params Key generation parameters.
 */
void keymgr_testutils_generate_versioned_key(
    const dif_keymgr_t *keymgr, const dif_keymgr_versioned_key_params_t params);

/**
 * Issues a keymgr disable and wait for it to complete
 */
void keymgr_testutils_disable(const dif_keymgr_t *keymgr);

/**
 * Polling keymgr status until it becomes idle.
 * Fail the test if the status code indicates any error.
 *
 * @param keymgr A key manager handle.
 */
void keymgr_testutils_wait_for_operation_done(const dif_keymgr_t *keymgr);

#endif  // OPENTITAN_SW_DEVICE_LIB_TESTING_KEYMGR_TESTUTILS_H_
