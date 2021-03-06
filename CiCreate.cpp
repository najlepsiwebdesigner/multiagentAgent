/* 
 * File:   CiCreate.cpp
 * Author: root
 * 
 * Created on Utorok, 2015, január 27, 16:45
 */

#include "CiCreate.h"

#define PI 3.14159265

void *Odometria(void *arg) {
    odometria_shm * shmO = (odometria_shm *) arg;

    // inicializujeme
    shmO->prejdena_vzdialenost = 0;
//    shmO->prejdeny_uhol = 0;
//    shmO->aktualny_uhol = 0;
    shmO->x_rel = 0;
    shmO->y_rel = 0;

    //cyklický update údajov
    while (!(shmO->ukonci_vlakno)) {
        shmO->mutCrdef.lock();
        auto t_start = std::chrono::high_resolution_clock::now();
        shmO->crDef->UpdateSensorsStates();
        shmO->prejdena_vzdialenost += abs(shmO->crDef->getLastDistance());
        shmO->prejdeny_uhol += abs(shmO->crDef->getLastAngle());
        shmO->aktualny_uhol += shmO->crDef->getLastAngle();
        shmO->aktualny_uhol = shmO->aktualny_uhol % 360;
        if (shmO->aktualny_uhol < 0) {
            shmO->aktualny_uhol += 360;
        }
        shmO->x_rel += shmO->crDef->getLastDistance() * sin(shmO->aktualny_uhol * PI / 180) * -1;
        shmO->y_rel += shmO->crDef->getLastDistance() * cos(shmO->aktualny_uhol * PI / 180);
        if (shmO->crDef->getWall()) {
            shmO->stena = 1;
        } else {
            shmO->stena = 0;
        }
        if (shmO->crDef->getBumpLeft()) {
            if (shmO->crDef->getBumpRight()) {
                shmO->naraznik_vpredu = 1;
                shmO->naraznik_vlavo = 0;
                shmO->naraznik_vpravo = 0;
            } else {
                shmO->naraznik_vpredu = 0;
                shmO->naraznik_vlavo = 1;
                shmO->naraznik_vpravo = 0;
            }
        } else {
            if (shmO->crDef->getBumpRight()) {
                shmO->naraznik_vpredu = 0;
                shmO->naraznik_vlavo = 0;
                shmO->naraznik_vpravo = 1;
            } else {
                shmO->naraznik_vpredu = 0;
                shmO->naraznik_vlavo = 0;
                shmO->naraznik_vpravo = 0;
            }
        }
        shmO->signalSteny = shmO->crDef->getWallSingal();
        shmO->bateriaNapatie = shmO->crDef->getBatteryNapatie();
        shmO->bateriaTeplota = shmO->crDef->getBatteryTeplota();
        shmO->Wheelpdrop = shmO->crDef->getZdvihnutie();
        shmO->Cliff = shmO->crDef->getSchody();
        shmO->mutCrdef.unlock();
        //výpis do terminálu
        //        system("clear");
        //        std::cout << "Celková prejdená vzdialenosť " << shm2->prejdena_vzdialenost << "\n";
        //        std::cout << "celkový uhol " << shm2->prejdeny_uhol << "\n";
        //        std::cout << "akt uhol " << shm2->aktualny_uhol << "\n";
        //        std::cout << "x_rel " << shm2->x_rel << "\n";
        //        std::cout << "y_rel " << shm2->y_rel << "\n";
        //        std::cout << "akt uhol " << shm2->aktualny_uhol << "\n";
        //        std::cout << "posledna vzdial " << shm2->crDef->getLastDistance() << "\n";
        //        std::cout << "posledny uhol " << shm2->crDef->getLastAngle() << "\n";
        //        std::cout << "naraznik vpredu " << shm2->naraznik_vpredu << "\n";
        //        std::cout << "naraznik vlavo " << shm2->naraznik_vlavo << "\n";
        //        std::cout << "naraznik vpravo " << shm2->naraznik_vpravo << "\n";
        //        std::cout << "stena " << shm2->stena << "\n";
        //        std::cout << "signal steny " << ((int) shm2->signalSteny) << "\n";
        //shm2->crDef->UpdateSensorsStates();
        usleep(40 * 1000);
        //usleep(150 * 1000);
        auto t_end = std::chrono::high_resolution_clock::now();
        /*std::cout << "odometria Wall clock time passed: "
                << std::chrono::duration<double, std::milli>(t_end - t_start).count()
                << " ms\n";*/
    }
    pthread_exit(NULL);
}

void *Gyro(void *arg) {
    std::cout << "vlakno gyro\n";
    odometria_shm * shmO = (odometria_shm *) arg;
    
    shmO->prejdeny_uhol = 0;
    shmO->aktualny_uhol = 0;
    
    int aktUhol = shmO->gyroComport->ReadGyro();
    int lastUhol = aktUhol;
    int diff;
    
    //cyklický update údajov
    while (!(shmO->ukonci_vlakno)) {
        //std::cout << "gyro\n";
        aktUhol = shmO->gyroComport->ReadGyro();
        diff = aktUhol - lastUhol;
        lastUhol = aktUhol;
        
        shmO->prejdeny_uhol += abs(diff);
        shmO->aktualny_uhol += diff;
        shmO->aktualny_uhol = shmO->aktualny_uhol % 360;
        if (shmO->aktualny_uhol < 0) {
            shmO->aktualny_uhol += 360;
        }
    }
    
    pthread_exit(NULL);
}

void *teleriadenie(void *arg) {
    std::cout << "teleriadenie\n";
    param_tele *param = (param_tele *) arg;
    char buf [50];

    while (!(*(param->ukonci)) && (param->create->getConnectedComport())) {
        //vypiseme stavy snimacov
        sprintf(buf, "%d", param->create->getPositionX());
        param->widget->xLabel->setText(buf);
        sprintf(buf, "%d", param->create->getPositionY());
        param->widget->yLabel->setText(buf);
        sprintf(buf, "%d", param->create->getPositionAngle());
        param->widget->zLabel->setText(buf);
        usleep(100 * 1000);
    }

    pthread_exit(NULL);
}

void posielaniePolohy(odometria_shm *shm_odo, komunikacia_shm *shm_R_GUI) {
    std::cout << "preskumaj prostredie\n";

    Position *poloha;
    Position *lastPoloha;
    Obstacle *prekazka;
    Obstacle *lastPrekazka;

    //inicializujeme
    poloha = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, shm_odo->x_rel, shm_odo->y_rel, shm_odo->aktualny_uhol);
    lastPoloha = poloha;
    shm_R_GUI->socket->sendJson(poloha->toJson());
    prekazka = new Obstacle(0, shm_R_GUI->id_spustenia, shm_R_GUI->id_prekazky, poloha, shm_odo->naraznik_vpravo, shm_odo->naraznik_vlavo, shm_odo->naraznik_vpredu);
    shm_R_GUI->socket->sendJson(prekazka->toJson());
    lastPrekazka = prekazka;

    while (1) {
        if (shm_R_GUI->ukonci_ulohu == true) {
            break;
        }
        poloha = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, shm_odo->x_rel, shm_odo->y_rel, shm_odo->aktualny_uhol);
        if (poloha->getDistance(lastPoloha) >= VZDIAL_POLOHY_POSLANIE) {
            shm_R_GUI->socket->sendJson(poloha->toJson());
            lastPoloha = poloha;
        }

        // ak sme narazeny posleme suradnicu prekazky
        if (((Agent *) shm_R_GUI->agent)->isInColision() && shm_R_GUI->isIdPrekazkyValid) {//aby bola pouzite dobre id prekazky
            Obstacle *prekazka = new Obstacle(0, shm_R_GUI->id_spustenia, shm_R_GUI->id_prekazky, poloha, shm_odo->naraznik_vpravo, shm_odo->naraznik_vlavo, shm_odo->naraznik_vpredu);
            if (prekazka->getDistance(lastPrekazka) >= VZDIAL_PREKAZKY_POSLANIE) {
                shm_R_GUI->prekazky->addObstacle(prekazka);
                shm_R_GUI->socket->sendJson(prekazka->toJson());
                lastPrekazka = prekazka;
            }
        }

        usleep(100 * 1000);
    }

    return;
}

CiCreate::CiCreate(komunikacia_shm *shm2) : Agent(shm2) {
}

int CiCreate::set_position(int x_0, int y_0, int uhol_0) {
    shm_odo->x_rel = x_0;
    shm_odo->y_rel = y_0;
    shm_odo->aktualny_uhol = uhol_0;
    std::cout << "nastav polohu\n";
    return 0;
}

int CiCreate::move(WORD p, WORD l) {
    //std::cout << "pohyb\n";

    if (connectedComport) {
        //saturujeme na min a max hodnoty rychlosti
        if (p < RIGHT_WHEEL_MAX_NEG_SPEED) p = RIGHT_WHEEL_MAX_NEG_SPEED;
        if (p > RIGHT_WHEEL_MAX_POS_SPEED) p = RIGHT_WHEEL_MAX_POS_SPEED;
        if (l < LEFT_WHEEL_MAX_NEG_SPEED) l = LEFT_WHEEL_MAX_NEG_SPEED;
        if (l > LEFT_WHEEL_MAX_POS_SPEED) l = LEFT_WHEEL_MAX_POS_SPEED;

        shm_odo->mutCrdef.lock();
        shm_odo->crDef->SendToCreate((unsigned char) 0x91, (WORD) p, (WORD) l);
        shm_odo->mutCrdef.unlock();
        return 0;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return -1;
    }

    return -1;
}

int CiCreate::connectComport(const char * comport) {
    if (this->connectedComport) {
        std::cout << "uz je pripojeny\n";
        return -1;
    }

    this->comport = comport;
    this->crDef = new CiCreateDef(comport, BAUDRATE);
    this->connectedComport = (crDef->ConnectToPort() == 0) ? true : false;

    if (this->connectedComport) {
        // alokovanie pamete pre SHM odometrie
        shm_odo = new odometria_shm;
        shm_odo->crDef = crDef;
        //inicializačné príkazy
        crDef->SendToCreate((unsigned char) OI_START);
        crDef->SendToCreate((unsigned char) OI_FULL);
        crDef->UpdateSensorsStates();
        //inicializácia vlastností
        shm_odo->prejdena_vzdialenost = 0;
        shm_odo->aktualny_uhol = 0;
        shm_odo->prejdeny_uhol = 0;
        shm_odo->x_rel;
        shm_odo->y_rel;
        shm_odo->ukonci_vlakno = 0;
        // vytvorí vlákno pre odometriu
        pthread_attr_t parametre;
        if (pthread_attr_init(&parametre)) return -1;
        pthread_attr_setdetachstate(&parametre, PTHREAD_CREATE_DETACHED);
        if (pthread_create(&vlaknoOdometria, &parametre, Odometria, (void*) shm_odo)) return -1;
        
        // vytvorí vlákno pre gyro
        /*shm_odo->gyroComport = new CComport("/dev/rfcomm0", B9600);
        shm_odo->gyroComport->OpenComport();
        pthread_attr_t parametre2;
        if (pthread_attr_init(&parametre2)) return -1;
        pthread_attr_setdetachstate(&parametre2, PTHREAD_CREATE_DETACHED);
        if (pthread_create(&vlaknoGyro, &parametre2, Gyro, (void*) shm_odo)) return -1;*/
        
        return 0;
    } else {
        return -1;
    }

}

int CiCreate::disConnectComport() {
    if (connectedComport) {
        move((WORD) 0, (WORD) 0);
        shm_odo->ukonci_vlakno = 1;
        usleep(100 * 1000);
        delete crDef;

        this->connectedComport = false;
        return 0;
    }
    return -1;
}

bool CiCreate::getConnectedComport() {
    return this->connectedComport;
}

int CiCreate::startTeleriadenie(void *widget) {
    stopTele = false;

    param_tele *param = new param_tele;
    param->ukonci = &stopTele;
    param->widget = shm_R_GUI->widget;
    param->create = (CiCreate *) shm_R_GUI->agent;

    // vytvorí vlákno pre teleriadenie
    pthread_attr_t parametre;
    if (pthread_attr_init(&parametre)) return -1;
    pthread_attr_setdetachstate(&parametre, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&vlaknoTeleriadenie, &parametre, teleriadenie, (void*) param)) return -1;

    return 0;
}

int CiCreate::stopTeleriadenie() {
    stopTele = true;
    usleep(200 * 1000);
    return 1;
    //pthread_cancel(vlaknoTeleriadenie);
}

int CiCreate::explore_environment() {
    std::cout << "preskumaj prostredie\n";
    shm_R_GUI->prekazky = new Obstacles();
    int iddd = shm_R_GUI->agent_id;
    shm_R_GUI->polohy = new PositionsOfAgents(iddd);

    // najskor spustime vlakno pre pravidelne posielanie polohy a prekazok
    posielanieThread = std::thread(posielaniePolohy, shm_odo, shm_R_GUI);

    AkcnyZasah *akcnyZasah = AkcnyZasah::stopNotObchadzanie();
    while (shm_R_GUI->ukonci_ulohu == false) {
        obchadzanie(akcnyZasah);
        skumanie(akcnyZasah);
        move(akcnyZasah);

        usleep(10 * 1000);
    }

    move(0, 0);
    shm_odo->wallFollowing = false;
    shm_R_GUI->isIdPrekazkyValid = false;

    //ukoncime odosielanie polohy a prekazok
    usleep(500 * 1000);
    posielanieThread.detach();
    //std::cout << shm_R_GUI->prekazky->toString();
    //std::cout << shm_R_GUI->polohy->toString();

    return 0;
}

AkcnyZasah * CiCreate::obchadzanie(AkcnyZasah *zasah) {
    //std::cout << "stav obchadzania " << zasah->GetObchadzanieStav() << "\n";
    switch (zasah->GetObchadzanieStav()) {
        case 0: //posleme robot dopredu ku stene
            zasah->SetLeftWheel(120);
            zasah->SetRightWheel(120);
            zasah->SetObchadzanieStav(1);
            break;
        case 1: //cakame kym nenarazi do prekazky
            if (shm_odo->naraznik_vpredu || shm_odo->naraznik_vlavo || shm_odo->naraznik_vpravo) {
                move(0, 0);
                zasah->SetLeftWheel(0);
                zasah->SetRightWheel(0);
                //zasah->SetObchadzanie(true);
                zasah->SetObchadzanieStav(2);
            }
            break;
        case 2: //natocime robot lepsie k prekazke
            if (shm_odo->naraznik_vlavo) {
                rotateAngle(60, 1);
            }
            if (shm_odo->naraznik_vpredu) {
                rotateAngle(30, 1);
            }
            zasah->SetLeftWheel(120);
            zasah->SetRightWheel(120);
            zasah->SetObchadzanieStav(3);
            break;
        case 3: //cakame kym nenarazi do prekazky
            if (shm_odo->naraznik_vpredu || shm_odo->naraznik_vlavo || shm_odo->naraznik_vpravo) {
                move(0, 0);
                zasah->SetLeftWheel(0);
                zasah->SetRightWheel(0);
                zasah->SetObchadzanieStav(4);
            }
            break;
        case 4: //chytime sa prekazky IR senzorom
            if (catchWall() == -1) {
                zasah->SetObchadzanie(false);
                zasah->SetObchadzanieStav(0);
            } else {
                Position *aktPoloha = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, shm_odo->x_rel, shm_odo->y_rel, shm_odo->aktualny_uhol);
                if (shm_R_GUI->prekazky->isNearAny(aktPoloha, 500)) {
                    std::cout << "obchadzanie - bol tu iny robot\n";
                    // najdeme najblizsiu prekazku a skorigujeme aktualnu polohu podla nej
                    Obstacle *nearestPrekazka = shm_R_GUI->prekazky->findNearest(aktPoloha);
                    shm_odo->x_rel = nearestPrekazka->GetX_rob();
                    shm_odo->y_rel = nearestPrekazka->GetY_rob();
                    
                    //resetujeme stav obchadzania
                    zasah->SetObchadzanie(false);
                    zasah->SetObchadzanieStav(0);
                } else {
                    shm_R_GUI->koorSur->setInvalid();
                    shm_R_GUI->socket->sendJson(SocketUtil::createInAccesibleCoorPosition(shm_R_GUI->agent_id));
                    zasah->SetObchadzanie(true);
                    zasah->SetObchadzanieStav(5);
                }
            }
            break;
        case 5: //vyziadame id prekazky
            shm_R_GUI->isIdPrekazkyValid = false;
            if (connectedIp) {
                int lastIdPrekazky = shm_R_GUI->id_prekazky;
                shm_R_GUI->socket->sendJson(SocketUtil::createNewIdOfObstacle());
                while (lastIdPrekazky == shm_R_GUI->id_prekazky && shm_R_GUI->ukonci_ulohu == false) {
                    //pockame kym nam pride nove
                    usleep(50 * 1000);
                }
                std::cout << "nove id prekazky: " << shm_R_GUI->id_prekazky << "\n";
            } else {
                //ak sme neni pripojení na server iba inkrementujeme
                shm_R_GUI->id_prekazky++;
                std::cout << "nove id prekazky: " << shm_R_GUI->id_prekazky << "\n";
            }
            shm_R_GUI->isIdPrekazkyValid = true;
            zasah->SetObchadzanieStav(6);
            break;
        case 6: //sledujeme stenu
            wallFollowingPure();
            zasah->SetObchadzanie(false);
            zasah->SetObchadzanieStav(0);
            //zasah->SetLeftWheel(0);
            //zasah->SetRightWheel(0);
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        default: //Optional
            zasah->SetObchadzanie(false);
            zasah->SetObchadzanieStav(0);
    }

    return zasah;

}

int CiCreate::catchWall() {
    int celk_uhol_0;

    celk_uhol_0 = shm_odo->prejdeny_uhol;

    move(100, -100);
    while (shm_odo->signalSteny < 50) {
        if (shm_R_GUI->ukonci_ulohu) {
            move(0, 0);
            return 0;
        }
        if (abs(celk_uhol_0 - shm_odo->prejdeny_uhol) > 200) {
            move(0, 0);
            return -1;
        }
        usleep(35 * 1000);
    }
}

int CiCreate::wallFollowingPure() {
    int uhol_0;
    int x_0;
    int y_0;
    int prejdena_vzdial;
    int vzdial = 0;
    int tolerancia;

    int minulyUhol = shm_odo->prejdeny_uhol;
    int vzdialenostRovno = shm_odo->prejdena_vzdialenost;

    int vzdialPoslSoStenou = shm_odo->prejdena_vzdialenost;

    // ulozime ze sledujeme stenu koli posielaniu suradnic prekazok
    shm_odo->wallFollowing = true;

    //ulozime pociatocnu polohu
    uhol_0 = shm_odo->aktualny_uhol;
    x_0 = shm_odo->x_rel;
    y_0 = shm_odo->y_rel;
    prejdena_vzdial = shm_odo->prejdena_vzdialenost;
    //obchádza na základe údajov z nárazníka a signálu steny
    while (1) {
        //ak narazí na stenu pred sebou otočí sa dolava        
        if (shm_odo->naraznik_vpredu) {
            rotateAngle(70, 1);
        }
        //ak narazí na stenu pred sebou otočí sa dolava        
        if (shm_odo->naraznik_vlavo) {
            rotateAngle(90, 1);
        }
        if ((shm_odo->stena == 0) && (shm_odo->naraznik_vpravo == 0)) {
            if (shm_odo->signalSteny > 10) {
                move(180, 200);
            } else {
                if (abs(vzdialPoslSoStenou - shm_odo->prejdena_vzdialenost) > 80) {
                    move(30, 230);
                } else {
                    move(180, 200);
                }
            }
        } else {
            if ((shm_odo->stena == 0) && (shm_odo->naraznik_vpravo == 1)) {
                if (shm_odo->signalSteny < 20) {
                    move(150, -90);
                } else {
                    move(200, 160);
                }
            } else {
                if ((shm_odo->stena == 1) && (shm_odo->naraznik_vpravo == 1)) {
                    move(190, 150);
                } else {
                    move(190, 170);
                }
            }
        }
        if (shm_odo->signalSteny > 10) {
            vzdialPoslSoStenou = shm_odo->prejdena_vzdialenost;
        }

        // ukončenie na základe požiadavky zo SHM
        if (shm_R_GUI->ukonci_ulohu) {
            move(0, 0);
            break;
        }
        // kazdy 1 meter tolerancia + 130mm
        tolerancia = (((shm_odo->prejdena_vzdialenost - prejdena_vzdial) / 1000)) * 35 + 200;
        if (tolerancia > 700) {
            tolerancia = 700;
        }
        vzdial = sqrt(((abs(x_0 - shm_odo->x_rel)) * (abs(x_0 - shm_odo->x_rel))) + ((abs(y_0 - shm_odo->y_rel)) * (abs(y_0 - shm_odo->y_rel))));
        //std::cout << "vzdial: " << vzdial << "toler: " << tolerancia << "uhol0" << uhol_0 << "\n";
        // ukončí ak sa vráti do počiatočného bodu
        if (((shm_odo->prejdena_vzdialenost - prejdena_vzdial) > 1000) && (abs(shm_odo->aktualny_uhol - uhol_0) < 35) && (vzdial < tolerancia)) {
            move(0, 0);
            Position *poloha0 = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, x_0, y_0, uhol_0);
            Obstacle *prekazka0 = new Obstacle(0, shm_R_GUI->id_spustenia, shm_R_GUI->id_prekazky, poloha0, 1, 0, 0);
            shm_R_GUI->socket->sendJson(prekazka0->toJson());
            std::cout << "prekazka obidena dookola\n";
            break;
        }
        // ukončí ak tu už bol iný robot
        Position *aktPoloha = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, shm_odo->x_rel, shm_odo->y_rel, shm_odo->aktualny_uhol);
        Obstacle *aktPrekazka = new Obstacle(0, shm_R_GUI->id_spustenia, shm_R_GUI->id_prekazky, aktPoloha, shm_odo->naraznik_vpravo, shm_odo->naraznik_vlavo, shm_odo->naraznik_vpredu);
        if (shm_R_GUI->prekazky->isNearOtherExceptId(aktPrekazka, (tolerancia*0.7))) {
            move(0, 0);
            Obstacle *prekazka1 = shm_R_GUI->prekazky->nearestObstacleExceptId(aktPrekazka);
            prekazka1->SetRobot(shm_R_GUI->agent_id);
            prekazka1->SetObstacle(shm_R_GUI->id_prekazky);
            shm_R_GUI->socket->sendJson(prekazka1->toJson());
            std::cout << "bol tu iny robot\n";
            std::cout << "aktualna " << aktPrekazka->toString() << "\n";
            std::cout << "dopocitana " << prekazka1->toString() << "\n";
            break;
        }

        // korekcia ak ide dlhsie rovno, nastavi sa natocenie na nasobok 90 stupnov
        int aktualnyUhol = shm_odo->prejdeny_uhol;

        if (abs(minulyUhol - aktualnyUhol) > 3) {
            minulyUhol = shm_odo->prejdeny_uhol;
            vzdialenostRovno = shm_odo->prejdena_vzdialenost;
            //std::cout << "resetujeme vzdialenost\n";
        }

        // korigujeme aktualne natocenie ak ideme popri stene
        if (shm_odo->prejdena_vzdialenost - vzdialenostRovno > 300) {
            //vyrovname uhol
            //std::cout << "Korigujeme uhol z \n" << shm_odo->aktualny_uhol;
            if ((shm_odo->aktualny_uhol > 325 && shm_odo->aktualny_uhol < 360) || (shm_odo->aktualny_uhol > 0 && shm_odo->aktualny_uhol < 35)) {
                shm_odo->aktualny_uhol = 0;
            }
            if (shm_odo->aktualny_uhol > 55 && shm_odo->aktualny_uhol < 125) {
                shm_odo->aktualny_uhol = 90;
                //std::cout << " na 90\n";
            }
            if (shm_odo->aktualny_uhol > 145 && shm_odo->aktualny_uhol < 205) {
                shm_odo->aktualny_uhol = 180;
                //std::cout << " na 180\n";
            }
            if (shm_odo->aktualny_uhol > 235 && shm_odo->aktualny_uhol < 305) {
                shm_odo->aktualny_uhol = 270;
                //std::cout << " na 270\n";
            }
            minulyUhol = shm_odo->prejdeny_uhol;
            vzdialenostRovno = shm_odo->prejdena_vzdialenost;

        }

        usleep(20 * 1000);
    }
    shm_odo->wallFollowing = false;
    shm_R_GUI->isIdPrekazkyValid = false;
    return 1;

}

AkcnyZasah * CiCreate::skumanie(AkcnyZasah *zasah) {
    // updatneme koor suradnicu
    //ak sme blizko bodu, dame ho ako neaktivny
    if (shm_R_GUI->koorSur->isValid() && !zasah->IsObchadzanie()) {
        if (sqrt(((shm_R_GUI->koorSur->GetY() - shm_odo->y_rel)*(shm_R_GUI->koorSur->GetY() - shm_odo->y_rel))
                + ((shm_R_GUI->koorSur->GetX() - shm_odo->x_rel)*(shm_R_GUI->koorSur->GetX() - shm_odo->x_rel))) < 200) {
            shm_R_GUI->koorSur->setInvalid();
            shm_R_GUI->socket->sendJson(SocketUtil::createInvalidCoorPosition(shm_R_GUI->agent_id));
            std::cout << "-------set invalid --------------- \n";
        }
    }

    // ak je invalid suradnica stojime
    if (!shm_R_GUI->koorSur->isValid()) {
        zasah->SetRightWheel(0);
        zasah->SetLeftWheel(0);
        return zasah;
    } else {
        // ak je validna suradnica a stoji, posleme ho dopredu
        if (zasah->GetRightWheel() < 50 && zasah->GetLeftWheel() < 50) {
            zasah->SetRightWheel(100);
            zasah->SetLeftWheel(100);
        }
    }

    if (shm_R_GUI->koorSur->isValid() && !zasah->IsObchadzanie()) {
        int uhol = angleToPosition(shm_R_GUI->koorSur->GetX(), shm_R_GUI->koorSur->GetY());
        //std::cout << "uhol k bodu " << uhol << "\n";

        if (uhol > 3) {
            /*if (uhol > 30) {
                zasah->SetRightWheel(zasah->GetRightWheel() + 15);
            } else {
                zasah->SetRightWheel(zasah->GetRightWheel() + 5);
            }*/
            zasah->SetRightWheel(200);
            zasah->SetLeftWheel(50);
            //std::cout << "left\n";
        } else if (uhol < -3) {
            /*if (uhol < -30) {
                zasah->SetLeftWheel(zasah->GetLeftWheel() + 15);
            } else {
                zasah->SetLeftWheel(zasah->GetLeftWheel() + 5);
            }*/
            zasah->SetRightWheel(50);
            zasah->SetLeftWheel(200);
            //std::cout << "right\n";
        } else {
            //obe dame na tu mensiu rychlost
            /*if (zasah->GetRightWheel() > zasah->GetLeftWheel()) {
                zasah->SetRightWheel(zasah->GetLeftWheel());
                zasah->SetLeftWheel(zasah->GetLeftWheel());
            } else {
                zasah->SetRightWheel(zasah->GetRightWheel());
                zasah->SetLeftWheel(zasah->GetRightWheel());
            }*/
            zasah->SetRightWheel(150);
            zasah->SetLeftWheel(150);
        }

    }
    return zasah;
}

int CiCreate::angleToPosition(int x, int y) {
    int uhol; //otoc sa o uhol k cielu

    int x_0; //vychodzia poloha
    int y_0;
    int uhol_0;

    float z1;
    float z2;
    float a2;
    float a1;

    float skal_sucin;
    float vek_sucin_z;
    float abs_a;
    float abs_z;
    float pom;

    x_0 = shm_odo->x_rel;
    y_0 = shm_odo->y_rel;
    uhol_0 = shm_odo->aktualny_uhol;

    z1 = x - x_0; //vektor ku zelanej polohe
    z2 = y - y_0;
    pom = uhol_0 + 90;
    if (pom > 360) pom = pom - 360;
    a1 = cos((pom) / 180 * PI); //y //vektor aktualneho natocenia
    a2 = sin((pom) / 180 * PI); //x

    skal_sucin = a1 * z1 + a2*z2;
    vek_sucin_z = a1 * z2 - a2*z1;
    abs_a = sqrt(a1 * a1 + a2 * a2);
    abs_z = sqrt(z1 * z1 + z2 * z2);

    uhol = acos(skal_sucin / abs_a / abs_z)*180 / PI;
    (vek_sucin_z > 0) ? uhol = uhol : uhol = uhol * (-1);

    return uhol;
}

int CiCreate::move(AkcnyZasah *zasah) {
    move(zasah->GetRightWheel(), zasah->GetLeftWheel());
}

int CiCreate::getPositionX() {
    return shm_odo->x_rel;
}

int CiCreate::getPositionY() {
    return shm_odo->y_rel;
}

int CiCreate::getPositionAngle() {
    return shm_odo->aktualny_uhol;
}

void CiCreate::pokusy() {
    //Dopredu_po_naraz();
    //shm_R_GUI->id_prekazky = 0;
    //Sledovanie_steny();
    //Otocenie_o_uhol_reg(359,0);

    explore_environment();
    std::cout << shm_R_GUI->prekazky->toString();
    std::cout << shm_R_GUI->polohy->toString();

    //    Dopredu_o_vzdialenost_reg(1000);
    //    Otocenie_o_uhol_reg(90,1);
    //    Dopredu_o_vzdialenost_reg(1000);
    //    Otocenie_o_uhol_reg(90,1);
    //    Dopredu_o_vzdialenost_reg(1000);
    //    Otocenie_o_uhol_reg(90,1);
    //    Dopredu_o_vzdialenost_reg(1000);
    //    Otocenie_o_uhol_reg(90,1);

    //    Dopredu_o_vzdialenost(1000);
    //    Otocenie_o_uhol(90,1);
    //    Dopredu_o_vzdialenost(1000);
    //    Otocenie_o_uhol(90,1);
    //    Dopredu_o_vzdialenost(1000);
    //    Otocenie_o_uhol(90,1);
    //    Dopredu_o_vzdialenost(1000);
    //    Otocenie_o_uhol(90,1);
}

int CiCreate::moveForwardTillHit() {
    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        move(100, 100);
        while (1) {
            if (shm_odo->naraznik_vpredu || shm_odo->naraznik_vlavo || shm_odo->naraznik_vpravo) {
                move(0, 0);
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }
        return 1;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return 0;
    }
}

int CiCreate::forwardDistance(int ziad_vzdial) {
    //postupne zrýchluje a spomaluje aby sa zmenšila chyba prešmykovaním kolies
    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        int poc_vzdial = shm_odo->prejdena_vzdialenost;
        // ak chceme prejst o vzdialenost vacsiu ako 200mm rozbiehame sa 
        // na 2 krat aby sme nepresmykovali, inak ideme len pomalsou rychlostou
        if (ziad_vzdial > 200) {
            ziad_vzdial += poc_vzdial;
            move(150, 150);
            while (1) {
                if (abs(shm_odo->prejdena_vzdialenost - poc_vzdial) > 10) {
                    break;
                }
                if (shm_R_GUI->ukonci_ulohu) {
                    move(0, 0);
                    break;
                }
                usleep(10 * 1000);
            }
            move(250, 250);
        } else {
            ziad_vzdial += poc_vzdial;
            move(150, 150);
        }
        //cakame kym sa priblizime na 70mm
        while (1) {
            if (abs(shm_odo->prejdena_vzdialenost - ziad_vzdial) < 70) {
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }
        //ked sme blizko zmensime rychlost aby sme sa presnejsie priblizili
        move(100, 100);
        while (1) {
            if (abs(shm_odo->prejdena_vzdialenost - ziad_vzdial) < 20) {
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }
        //sme na mieste zastavime
        move(0, 0);
        return 1;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return 0;
    }
}

int CiCreate::forwardDistanceReg(int ziad_vzdial) {
    //postupne zrýchluje a potom regulatorom zastavi na pozadovanej vzdialenosti
    int K = 2;
    int rychl = 50;
    int akcZasah;

    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        ziad_vzdial = abs(ziad_vzdial);
        int poc_vzdial = shm_odo->prejdena_vzdialenost;
        ziad_vzdial += poc_vzdial;

        // rozbiehame az dokedy sa nedostaneme na rychlost aktualneho akcneho zasahu
        while (1) {
            rychl += 10 * K;
            akcZasah = K * (ziad_vzdial - shm_odo->prejdena_vzdialenost);

            move((WORD) rychl, (WORD) rychl);

            if (rychl >= abs(akcZasah) || abs(rychl) > LEFT_WHEEL_MAX_POS_SPEED) {
                break;
            }

            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }

        //brzdime
        while (1) {
            akcZasah = K * (ziad_vzdial - shm_odo->prejdena_vzdialenost);
            // min rychlost 50
            if (abs(akcZasah < 50)) {
                if (akcZasah > 0) akcZasah = 50;
                if (akcZasah < 0) akcZasah = -50;
            }

            move((WORD) akcZasah, (WORD) akcZasah);

            if (abs(shm_odo->prejdena_vzdialenost - ziad_vzdial) < 10) {
                break;
            }

            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }
        //sme na mieste zastavime
        move(0, 0);
        return 1;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return 0;
    }
}

int CiCreate::rotateAngle(int ziad_uhol, int smer) {
    // 0 doprava
    // 1 dolava

    //postupne zrýchluje a spomaluje aby sa zmenšila chyba prešmykovaním kolies
    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        int pociatocny_uhol = shm_odo->prejdeny_uhol;
        ziad_uhol = ziad_uhol % 360;
        if (ziad_uhol < 0) {
            ziad_uhol = -ziad_uhol;
            smer = (smer) ? 0 : 1;
        }
        if (ziad_uhol > 30) {
            ziad_uhol += shm_odo->prejdeny_uhol;
            if (smer) {
                move(70, -70);
            } else {
                move(-70, 70);
            }
            while (1) {
                if (abs(shm_odo->prejdeny_uhol - pociatocny_uhol) > 4) {
                    break;
                }
                if (shm_R_GUI->ukonci_ulohu) {
                    move(0, 0);
                    break;
                }
                usleep(10 * 1000);
            }
            if (smer) {
                move(150, -150);
            } else {
                move(-150, 150);
            }
        } else {
            ziad_uhol += shm_odo->prejdeny_uhol;
            if (smer) {
                move(45, -45);
            } else {
                move(-45, 45);
            }

        }
        while (1) {
            if (abs((shm_odo->prejdeny_uhol) - (ziad_uhol)) < 30) {
                if (smer) {
                    move(80, -80);
                } else {
                    move(-80, 80);
                }
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(30000);
        }
        while (1) {
            if (abs((shm_odo->prejdeny_uhol) - (ziad_uhol)) < 15) {
                if (smer) {
                    move(45, -45);
                } else {
                    move(-45, 45);
                }
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(30000);
        }
        while (1) {
            if (abs((shm_odo->prejdeny_uhol) - (ziad_uhol)) < 7) {
                move(0, 0);
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(30000);
        }
        return 1;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return 0;
    }
}

int CiCreate::rotateAngleReg(int ziad_uhol, int smer) {
    // 0 doprava
    // 1 dolava

    // rozbiehame az dokedy sa nedostaneme na rychlost aktualneho akcneho zasahu
    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        float e; //reg odchylka
        float P = 6;
        int zasah;
        int rychl = 50;

        int pociatocny_uhol = shm_odo->prejdeny_uhol;
        ziad_uhol = ziad_uhol % 360;
        if (ziad_uhol < 0) {
            ziad_uhol = -ziad_uhol;
            smer = (smer) ? 0 : 1;
        }
        ziad_uhol += shm_odo->prejdeny_uhol;

        while (1) {
            rychl += 2 * P;
            e = ziad_uhol - shm_odo->prejdeny_uhol;
            zasah = e*P;

            if (smer) {
                move(rychl, -1 * rychl);
            } else {
                move(-1 * rychl, rychl);
            }

            if (rychl >= abs(zasah) || abs(rychl) > LEFT_WHEEL_MAX_POS_SPEED) {
                break;
            }

            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(10 * 1000);
        }

        //brzdime
        while (1) {
            e = ziad_uhol - shm_odo->prejdeny_uhol;
            zasah = e*P;
            if (smer) {
                move((WORD) zasah, (WORD) - 1 * zasah);
            } else {
                move((WORD) - 1 * zasah, (WORD) zasah);
            }
            if (abs((shm_odo->prejdeny_uhol) - (ziad_uhol)) < 2) {
                move(0, 0);
                break;
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            usleep(30000);
        }
        return 1;
    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        return 0;
    }
}

int CiCreate::wallFollowing() {
    int uhol_0;
    int celk_uhol_0;
    int x_0;
    int y_0;
    int prejdena_vzdial;
    int vzdial = 0;
    int id_prekazky;
    int tolerancia;

    if (connectedComport && shm_R_GUI->ukonci_ulohu == false) {
        if (shm_R_GUI->ukonci_ulohu) {
            move(0, 0);
            return 1;
        }
        moveForwardTillHit();

        // ulozime ze sledujeme stenu koli posielaniu suradnic prekazok
        shm_odo->wallFollowing = true;

        celk_uhol_0 = shm_odo->prejdeny_uhol;
        if (shm_odo->naraznik_vlavo) {
            rotateAngle(60, 1);
        }
        if (shm_odo->naraznik_vpredu) {
            rotateAngle(30, 1);
        }
        moveForwardTillHit();
        //chytí sa steny (natočí sa aby chytil signál steny)
        while (shm_odo->signalSteny < 35) {
            move(100, -100);
            if (shm_odo->naraznik_vpravo == 0) {
                //Dopredu_po_naraz();
            }
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                shm_odo->wallFollowing = false;
                return 1;
            }
            if (abs(celk_uhol_0 - shm_odo->prejdeny_uhol) > 200) {
                move(0, 0);
                shm_odo->wallFollowing = false;
                return 0;
            }
            usleep(35 * 1000);
        }
        //vyžiada id prekážky
        shm_R_GUI->isIdPrekazkyValid = false;
        if (connectedIp) {
            int lastIdPrekazky = shm_R_GUI->id_prekazky;
            shm_R_GUI->socket->sendJson(SocketUtil::createNewIdOfObstacle());
            while (lastIdPrekazky == shm_R_GUI->id_prekazky && shm_R_GUI->ukonci_ulohu == false) {
                //pockame kym nam pride nove
                usleep(50 * 1000);
            }
            std::cout << "nove id prekazky: " << shm_R_GUI->id_prekazky << "\n";
        } else {
            //ak sme neni pripojení na server iba inkrementujeme
            shm_R_GUI->id_prekazky++;
            std::cout << "nove id prekazky: " << shm_R_GUI->id_prekazky << "\n";
        }
        shm_R_GUI->isIdPrekazkyValid = true;
        //ulozime pociatocnu polohu
        uhol_0 = shm_odo->aktualny_uhol;
        x_0 = shm_odo->x_rel;
        y_0 = shm_odo->y_rel;
        prejdena_vzdial = shm_odo->prejdena_vzdialenost;
        //obchádza na základe údajov z nárazníka a signálu steny
        while (1) {
            //ak narazí na stenu pred sebou otočí sa dolava        
            if (shm_odo->naraznik_vpredu) {
                rotateAngle(70, 1);
            }
            //mení zakrivenie pohybu podla pozície k stene
            if ((shm_odo->stena == 0) && (shm_odo->naraznik_vpravo == 0)) {
                if (shm_odo->signalSteny < 20) {
                    move(30, 230);
                } else {
                    move(180, 200);
                }
            } else {
                if ((shm_odo->stena == 0) && (shm_odo->naraznik_vpravo == 1)) {
                    if (shm_odo->signalSteny < 20) {
                        move(150, -90);
                    } else {
                        move(200, 160);
                    }
                } else {
                    move(190, 170);
                }
            }
            // ukončenie na základe požiadavky zo SHM
            if (shm_R_GUI->ukonci_ulohu) {
                move(0, 0);
                break;
            }
            // kazde 2 metre tolerancia + 130mm
            tolerancia = (((shm_odo->prejdena_vzdialenost - prejdena_vzdial) / 2000) + 1) * 130;
            vzdial = sqrt(((abs(x_0 - shm_odo->x_rel)) * (abs(x_0 - shm_odo->x_rel))) + ((abs(y_0 - shm_odo->y_rel)) * (abs(y_0 - shm_odo->y_rel))));
            //std::cout << "vzdial: " << vzdial << "toler: " << tolerancia << "\n";
            // ukončí ak sa vráti do počiatočného bodu
            if (((shm_odo->prejdena_vzdialenost - prejdena_vzdial) > 1000) && (abs(shm_odo->aktualny_uhol - uhol_0) < 40) && (vzdial < tolerancia)) {
                move(0, 0);
                break;
            }
            // ukončí ak tu už bol iný robot
            Position *aktPoloha = new Position(0, shm_R_GUI->id_spustenia, shm_R_GUI->agent_id, shm_odo->x_rel, shm_odo->y_rel, shm_odo->aktualny_uhol);
            Obstacle *aktPrekazka = new Obstacle(0, shm_R_GUI->id_spustenia, shm_R_GUI->id_prekazky, aktPoloha, shm_odo->naraznik_vpravo, shm_odo->naraznik_vlavo, shm_odo->naraznik_vpredu);
            if (shm_R_GUI->prekazky->isNearOtherExceptId(aktPrekazka, (tolerancia + 300))) {
                move(0, 0);
                break;
            }
            usleep(40 * 1000);
        }
        shm_odo->wallFollowing = false;
        shm_R_GUI->isIdPrekazkyValid = false;
        return 1;

    } else {
        std::cout << "Nemožno spustit pohyb po naraz, neni pripojený robot\n";
        shm_odo->wallFollowing = false;
        return -1;
    }
}

int CiCreate::getWallSensor() {
    int pom = shm_odo->signalSteny;
    return pom;
}

bool CiCreate::isWall() {
    return shm_odo->stena == 1 ? true : false;
}

bool CiCreate::isInColision() {
    return (shm_odo->naraznik_vlavo == 1 ||
            shm_odo->naraznik_vpravo == 1 ||
            shm_odo->naraznik_vpredu == 1 ||
            shm_odo->wallFollowing);
}

CiCreate::~CiCreate() {
    std::cout << "destruktor CiCreate\n";
    disConnectComport();
}

