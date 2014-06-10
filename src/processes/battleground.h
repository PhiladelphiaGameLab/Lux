/*
 * This code was orginally written and developed by the Lux Backend Team
 * at Philadelphia Game Lab:
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * - Jake
 */
#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H
#define DATABASE_NAME "Gameplay_DB"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>
#include <cstdlib>
#include <iostream>

#include <mutex>

#include <netinet/in.h>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

#include "luxSocket.h"

#include "Authenticate.h"

#include "HMBL.h"
#include "sendupdate.h"
#include <time.h>

using namespace mongo;
using namespace std;
using namespace socketlibrary;


// static class (thread)

struct s_SUT{
    string pipeLocation;
};

struct s_BGT{
    int port;
};

struct s_bgt_params_in
{
    char* pipe_w;
    char* pipe_hmbl;
};

class BattleGround{
    public:
        static void spawn(s_bgt_params_in params_in);
        static uint16_t getNewPort();
    protected:
    private:
};

#endif // BATTLEGROUND_H
