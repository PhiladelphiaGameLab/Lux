#ifndef BGTSPAWNER_H
#define BGTSPAWNER_H
#ifndef DATABASE_NAME
#define DATABASE_NAME "Luxdb.gameplay"
#endif
#include "battleground.h"
#include "sendupdate.h"
#include "SendNewRelevant.h"
#include "DBWriter.h"

class BGTSpawner
{
    public:
        bool spawnNewBgt(int);
    protected:
    private:
};

#endif // BGTSPAWNER_H
