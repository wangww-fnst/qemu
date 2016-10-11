/*
 * COLO Implementation
 *
 * Copyright Fujitsu, Corp. 2015
 *
 * Authors:
 *     Wen Congyang <wency@cn.fujitsu.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#include "qemu/osdep.h"
#include "qemu-version.h"
#include "qemu/cutils.h"
#include "monitor/monitor.h"
#include "sysemu/sysemu.h"
#include "qmp-commands.h"
#include "sysemu/char.h"
#include "ui/qemu-spice.h"
#include "ui/vnc.h"
#include "sysemu/kvm.h"
#include "sysemu/arch_init.h"
#include "hw/qdev.h"
#include "sysemu/blockdev.h"
#include "sysemu/block-backend.h"
#include "qom/qom-qobject.h"
#include "qapi/qmp/qerror.h"
#include "qapi/qmp/qobject.h"
#include "qapi/qmp-input-visitor.h"
#include "hw/boards.h"
#include "qom/object_interfaces.h"
#include "hw/mem/pc-dimm.h"
#include "hw/acpi/acpi_dev_interface.h"
#include "replication.h"

void qmp_xen_set_replication(bool enable, bool primary,
                                   bool has_failover, bool failover,
                                   Error **errp)
{
    ReplicationMode mode = primary ? REPLICATION_MODE_PRIMARY : REPLICATION_MODE_SECONDARY;
    if (has_failover && enable) {
        error_setg(errp, "Parameter '%s' is only for stopping replication",
                   "failover");
    }

    if (enable) {
        replication_start_all(mode, errp);
    } else {
        replication_stop_all(failover, failover ? NULL : errp);
    }
}

void qmp_xen_get_replication_error(Error **errp)
{
    replication_get_error_all(errp);
}

void qmp_xen_do_checkpoint(Error **errp)
{
    replication_do_checkpoint_all(errp);
}
