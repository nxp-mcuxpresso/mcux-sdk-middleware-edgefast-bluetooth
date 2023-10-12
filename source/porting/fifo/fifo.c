
/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "porting.h"

#include "fifo.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_FIFO)
#define LOG_MODULE_NAME bt_fifo
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

void bt_fifo_init(bt_fifo_t *fifo)
{
    osa_status_t ret;

    if (NULL == fifo->sem)
    {
        LOG_DBG("FIFO init", fifo);

        ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)fifo->semHandle, 0);

        if (KOSA_StatusSuccess == ret)
        {
            fifo->sem = (osa_semaphore_handle_t)fifo->semHandle;
            bt_list_init(&fifo->list);
        }
        else
        {
            LOG_ERR("failed %d", ret);
        }
    }
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_append_debug(bt_fifo_t *fifo, void *data, const char *func, int line)
#else
void bt_fifo_append(bt_fifo_t *fifo, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    osa_status_t ret;

    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    LOG_INF("node %p, fifo %p", data, fifo);
    bt_list_append(&fifo->list, (bt_list_node_t*)data);
    LOG_INF("next %p", ((bt_list_node_t*)data)->next);
    ret = OSA_SemaphorePost(fifo->sem);
#if 0
    assert(KOSA_StatusSuccess == ret);
#endif
    (void)ret;
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_prepend_debug(bt_fifo_t *fifo, void *data, const char *func, int line)
#else
void bt_fifo_prepend(bt_fifo_t *fifo, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    osa_status_t ret;

    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    bt_list_prepend(&fifo->list, (bt_list_node_t*)data);
    ret = OSA_SemaphorePost(fifo->sem);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;
}

#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
void bt_fifo_insert_debug(bt_fifo_t *fifo, void *prev, void *data, const char *func, int line)
#else
void bt_fifo_insert(bt_fifo_t *fifo, void *prev, void *data)
#endif /* CONFIG_BT_DEBUG_FIFO */
{
    osa_status_t ret;

    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
#if (defined(CONFIG_BT_DEBUG_FIFO) && (CONFIG_BT_DEBUG_FIFO > 0U))
        LOG_ERR("%s():%d:node %p has been inserted to fifo %p", func, line, data, fifo);
#else
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
#endif /* CONFIG_BT_DEBUG_FIFO */
        return;
    }
    bt_list_insert(&fifo->list, (bt_list_node_t*)prev, (bt_list_node_t*)data);
    ret = OSA_SemaphorePost(fifo->sem);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;
}

int bt_fifo_append_list(bt_fifo_t *fifo, void *head, void *tail)
{
    osa_status_t ret;

    bt_fifo_init(fifo);
    bt_list_append_list(&fifo->list, (bt_list_node_t*)head, (bt_list_node_t*)tail);

    ret = OSA_SemaphorePost(fifo->sem);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;

    return 0;
}

void *bt_fifo_get(bt_fifo_t *fifo, size_t timeout)
{
    bt_list_node_t *head = NULL;
    osa_status_t ret;
    uint32_t start = OSA_TimeGetMsec();
    uint32_t current = start;

    bt_fifo_init(fifo);

    while (((current - start) <= timeout) && (NULL == head))
    {
        head = bt_list_get(&fifo->list);
        if (NULL == head)
        {
            if ((current - start) < timeout)
            {
                ret = OSA_SemaphoreWait(fifo->sem, timeout - (current - start));
                assert(KOSA_StatusSuccess == ret);
                (void)ret;
            }
        }
        else
        {
            LOG_INF("node %p, fifo %p", head, fifo);
        }
        current = OSA_TimeGetMsec();
    }
    return head;
}

bool bt_fifo_remove(bt_fifo_t *fifo, void *data)
{
    bt_fifo_init(fifo);

    return bt_list_find_and_remove(&fifo->list, (bt_list_node_t*)data);
}

bool bt_fifo_unique_append(bt_fifo_t *fifo, void *data)
{
    osa_status_t ret;

    bt_fifo_init(fifo);
    if (true == bt_list_find(&fifo->list, (bt_list_node_t*)data))
    {
        LOG_ERR("node %p has been inserted to fifo %p", data, fifo);
        return false;
    }
    bt_list_append(&fifo->list, (bt_list_node_t*)data);
    ret = OSA_SemaphorePost(fifo->sem);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;
    return true;
}

int bt_fifo_is_empty(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_is_empty(&fifo->list);
}

void *bt_fifo_peek_head(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_peek_head(&fifo->list);
}

void *bt_fifo_peek_tail(bt_fifo_t *fifo)
{
    bt_fifo_init(fifo);

    return bt_list_peek_tail(&fifo->list);
}