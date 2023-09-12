#pragma once

// THIS IS USED FOR TEMPLATING PURPOSES ONLY. DO NOT CHANGE THIS.

const char* CONFIG_TEMPLATE_FILE_NAME = "server.conf";

const char* CONFIG_TEMPLATE_STRING =
"# --- \n" \
"HOST=127.0.0.1\n" \
"# --- \n" \
"SQUARE_PREFIX=Square\n" \
"SQUARE_CAPACITY=50\n" \
"# --- \n" \
"GAME_REVISION=20120314\n" \
"GAME_WHITELIST=1\n" \
"GAME_PORT=8020\n" \
"GAME_ENABLED=1\n" \
"GAME_MAX_ROOMS=30\n" \
"# --- \n" \
"EVENT_ACTIVE=1\n" \
"GLOBAL_EXP_RATE=100\n" \
"GLOBAL_DON_RATE=150\n" \
"# --- \n" \
"LOGIN_ATTEMPT_COOLDOWN=0\n" \
"MAX_FAILED_LOGIN_ATTEMPTS=10\n" \
"ACCOUNT_IP_LOCK=0\n" \
"# --- \n" \
"COMMAND_PREFIX=+\n"\
"# --- \n" \
"DB_HOST=127.0.0.1\n" \
"DB_PORT=3306\n" \
"DB_NAME=qpang\n" \
"DB_USER=root\n" \
"DB_PASS=\n" \
;
