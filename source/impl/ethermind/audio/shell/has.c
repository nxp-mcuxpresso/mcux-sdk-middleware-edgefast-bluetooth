/**
 * @file
 * @brief Bluetooth Hearing Access Service (HAS) shell.
 *
 * Copyright (c) 2022 Codecoup
 * Copyright (C) 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_HAS_CLIENT) && (CONFIG_BT_HAS_CLIENT > 0))

#include <bluetooth/conn.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/audio/has.h>
#include "fsl_shell.h"
#include <stdlib.h>
#include <stdio.h>

#include "shell_bt.h"

static int preset_select(uint8_t index, bool sync)
{
	return 0;
}

static void preset_name_changed(uint8_t index, const char *name)
{
	shell_print(ctx_shell, "Preset name changed index %u name %s", index, name);
}

static const struct bt_has_preset_ops preset_ops = {
	.select = preset_select,
	.name_changed = preset_name_changed,
};

static shell_status_t cmd_preset_reg(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	struct bt_has_preset_register_param param = {
		.index = shell_strtoul(argv[1], 16, &err),
		.properties = (enum bt_has_properties)shell_strtoul(argv[2], 16, &err),
		.name = argv[3],
		.ops = &preset_ops,
	};

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_register(&param);
	if (err < 0) {
		shell_error(shell, "Preset register failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_unreg(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	const uint8_t index = shell_strtoul(argv[1], 16, &err);

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_unregister(index);
	if (err < 0) {
		shell_print(shell, "Preset unregister failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_features_set(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	struct bt_has_features_param param = {
		.type = BT_HAS_HEARING_AID_TYPE_MONAURAL,
		.preset_sync_support = false,
		.independent_presets = false
	};

	for (size_t argn = 1; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (strcmp(arg, "binaural") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_BINAURAL;
		} else if (strcmp(arg, "monaural") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_MONAURAL;
		} else if (strcmp(arg, "banded") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_BANDED;
		} else if (strcmp(arg, "sync") == 0) {
			param.preset_sync_support = true;
		} else if (strcmp(arg, "independent") == 0) {
			param.independent_presets = true;
		} else {
			shell_help(shell);
			return kStatus_SHELL_Error;
		}
	}

	err = bt_has_features_set(&param);
	if (err != 0) {
		shell_error(shell, "Could not set features: %d", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_has_register(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;
	struct bt_has_features_param param = {
		.type = BT_HAS_HEARING_AID_TYPE_MONAURAL,
		.preset_sync_support = false,
		.independent_presets = false
	};

	for (size_t argn = 1; argn < argc; argn++) {
		const char *arg = argv[argn];

		if (strcmp(arg, "binaural") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_BINAURAL;
		} else if (strcmp(arg, "monaural") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_MONAURAL;
		} else if (strcmp(arg, "banded") == 0) {
			param.type = BT_HAS_HEARING_AID_TYPE_BANDED;
		} else if (strcmp(arg, "sync") == 0) {
			param.preset_sync_support = true;
		} else if (strcmp(arg, "independent") == 0) {
			param.independent_presets = true;
		} else {
			shell_help(shell);
			return kStatus_SHELL_Error;
		}
	}

	err = bt_has_register(&param);
	if (err != 0) {
		shell_error(shell, "Could not register HAS: %d", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

struct print_list_entry_data {
	int num;
	shell_handle_t shell;
};

static uint8_t print_list_entry(uint8_t index, enum bt_has_properties properties,
				const char *name, void *user_data)
{
	struct print_list_entry_data *data = user_data;

	shell_print(data->shell, "%d: index 0x%02x prop 0x%02x name %s", ++data->num, index,
		    properties, name);

	return BT_HAS_PRESET_ITER_CONTINUE;
}

static shell_status_t cmd_preset_list(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct print_list_entry_data data = {
		.shell = shell,
	};

	bt_has_preset_foreach(0, print_list_entry, &data);

	if (data.num == 0) {
		shell_print(shell, "No presets registered");
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_avail(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	const uint8_t index = shell_strtoul(argv[1], 16, &err);

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_available(index);
	if (err < 0) {
		shell_print(shell, "Preset availability set failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_unavail(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	const uint8_t index = shell_strtoul(argv[1], 16, &err);

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_unavailable(index);
	if (err < 0) {
		shell_print(shell, "Preset availability set failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_active_set(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	const uint8_t index = shell_strtoul(argv[1], 16, &err);

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_active_set(index);
	if (err < 0) {
		shell_print(shell, "Preset selection failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_active_get(shell_handle_t shell, int32_t argc, char *argv[])
{
	const uint8_t index = bt_has_preset_active_get();

	shell_print(shell, "Active index 0x%02x", index);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_active_clear(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err;

	err = bt_has_preset_active_clear();
	if (err < 0) {
		shell_print(shell, "Preset selection failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_preset_name_set(shell_handle_t shell, int32_t argc, char *argv[])
{
	int err = 0;
	const uint8_t index = shell_strtoul(argv[1], 16, &err);

	if (err < 0) {
		shell_print(shell, "Invalid command parameter (err %d)", err);
		return (shell_status_t)err;
	}

	err = bt_has_preset_name_change(index, argv[2]);
	if (err < 0) {
		shell_print(shell, "Preset name change failed (err %d)", err);
		return (shell_status_t)err;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_has(shell_handle_t shell, int32_t argc, char *argv[])
{
	if (argc > 1) {
		shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);
	} else {
		shell_error(shell, "%s missing subcomand", argv[0]);
	}

	return kStatus_SHELL_Error;
}

SHELL_STATIC_SUBCMD_SET_CREATE(has_cmds,
	SHELL_CMD_ARG(register, NULL,
		      "Initialize the service and register type "
		      "[binaural | monaural(default) | banded] [sync] [independent]",
		      cmd_has_register, 1, 3),
	SHELL_CMD_ARG(preset-reg, NULL, "Register preset <index> <properties> <name>",
		      cmd_preset_reg, 4, 0),
	SHELL_CMD_ARG(preset-unreg, NULL, "Unregister preset <index>", cmd_preset_unreg, 2, 0),
	SHELL_CMD_ARG(preset-list, NULL, "List all presets", cmd_preset_list, 1, 0),
	SHELL_CMD_ARG(preset-set-avail, NULL, "Set preset as available <index>",
		      cmd_preset_avail, 2, 0),
	SHELL_CMD_ARG(preset-set-unavail, NULL, "Set preset as unavailable <index>",
		      cmd_preset_unavail, 2, 0),
	SHELL_CMD_ARG(preset-active-set, NULL, "Set active preset <index>",
		      cmd_preset_active_set, 2, 0),
	SHELL_CMD_ARG(preset-active-get, NULL, "Get active preset", cmd_preset_active_get, 1, 0),
	SHELL_CMD_ARG(preset-active-clear, NULL, "Clear selected preset",
		      cmd_preset_active_clear, 1, 0),
	SHELL_CMD_ARG(set-name, NULL, "Set preset name <index> <name>", cmd_preset_name_set, 3, 0),
	SHELL_CMD_ARG(features-set, NULL, "Set hearing aid features "
		      "[binaural | monaural(default) | banded] [sync] [independent]",
		      cmd_features_set, 1, 3),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(has, has_cmds, "Bluetooth HAS shell commands", cmd_has, 1, 1);

void bt_ShellHasInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandhas))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandhas.pcCommand);
    }
}

#endif /* CONFIG_BT_HAS_CLIENT */