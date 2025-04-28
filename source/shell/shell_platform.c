/** @file
 * @brief Bluetooth shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <porting.h>

#include "shell_bt.h"
#if (defined(CONFIG_BT_PLATFORM_POWER_SWITCH) && (CONFIG_BT_PLATFORM_POWER_SWITCH > 0))
#include "fwk_platform_ble.h"
#endif /* CONFIG_BT_PLATFORM_POWER_SWITCH */

#if (defined(CONFIG_BT_PLATFORM_POWER_SWITCH) && (CONFIG_BT_PLATFORM_POWER_SWITCH > 0))
static int bt_power_off(void)
{
	bt_disable();
	return PLATFORM_TerminateBle();
}

static int bt_power_on(void)
{
	int err;
	err = PLATFORM_InitBle();
	if (0 != err)
	{
		return err;
	}
	return 0;
}

static int cmd_power_switch(const struct shell *sh, size_t argc, char *argv[])
{
	int err;
	const char *action;
	action = argv[1];
	if (!strcmp(action, "on")) {
		err = bt_power_on();
		if (err) {
			shell_error(sh, "bt power on failed (err %d)", err);
			return err;
		}
		shell_print(sh, "bt power on success");
		return err;
	} else if (!strcmp(action, "off")) {
		err = bt_power_off();
		if (err) {
			shell_error(sh, "bt power off failed (err %d)", err);
			return err;
		}
		shell_print(sh, "bt power off success ");
		return 0;
	} else {
		shell_help(sh);
		return SHELL_CMD_HELP_PRINTED;
	}
}
#endif /* CONFIG_BT_PLATFORM_POWER_SWITCH */

SHELL_STATIC_SUBCMD_SET_CREATE(platform_cmds,
#if (defined(CONFIG_BT_PLATFORM_POWER_SWITCH) && (CONFIG_BT_PLATFORM_POWER_SWITCH > 0))
	SHELL_CMD_ARG(power_switch, NULL, "<value: on, off>", cmd_power_switch, 2, 0),
#endif /* CONFIG_BT_PLATFORM_POWER_SWITCH */
	SHELL_SUBCMD_SET_END
);

static int cmd_platform(const struct shell *sh, size_t argc, char **argv)
{
	if (argc == 1) {
		shell_help(sh);
		/* shell returns 1 when help is printed */
		return SHELL_CMD_HELP_PRINTED;
	}

	shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

	return -ENOEXEC;
}

SHELL_CMD_ARG_REGISTER(platform, &platform_cmds, "Bluetooth PLATFORM shell commands",
		       cmd_platform, 1, 1);

void bt_ShellPlatformInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandplatform))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandplatform.pcCommand);
    }
}
