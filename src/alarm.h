/**
 * @file alarm.h
 * @author Mislav Novakovic <mislav.novakovic@sartur.hr>
 * @brief header file for alarm.c.
 *
 * @copyright
 * Copyright (C) 2017 Deutsche Telekom AG.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ALARM_H
#define ALARM_H

#include <sysrepo.h>
#include <sys/queue.h>
#include <libyang/libyang.h>

#define XPATH_MAX_LEN 128
#define UNIX_PATH_MAX 256

typedef struct ctx_s {
	const char *yang_model;
    struct ly_ctx *libyang_ctx;
	const struct lys_module *module;
	int pid;
	int socket_fd;
	char socket_path[UNIX_PATH_MAX];
	sr_subscription_ctx_t *sub;
	sr_session_ctx_t *sess;
	sr_session_ctx_t *running_sess;
	sr_conn_ctx_t *startup_conn;
	sr_session_ctx_t *startup_sess;
} ctx_t;

#endif /* ALARM_H */
