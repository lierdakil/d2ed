#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <bitset>
#include "d2struct.h"

#define ACTS 6

class d2s {
public:
    enum propId {Strength,
                 Energy,
                 Dexterity,
                 Vitality,
                 Stat_Points,
                 Skill_Points,
                 Current_Life,
                 Maximum_Life,
                 Current_Mana,
                 Maximum_Mana,
                 Current_Stamina,
                 Maximum_Stamina,
                 Level,
                 Experience,
                 Gold,
                 Gold_Stash};
private:
    QByteArray data_;

    int propLen(propId type) {
        switch(type) {
        case Strength://Str
        case Energy://Enrg
        case Dexterity://Dex
        case Vitality://Vita
        case Stat_Points://Free Stat
            return 10;
        case Skill_Points:
            return 8;
        case Current_Life:
        case Maximum_Life:
        case Current_Mana:
        case Maximum_Mana:
        case Current_Stamina:
        case Maximum_Stamina:
            return 21;
        case Level:
            return 7;
        case Experience:
            return 32;
        case Gold:
        case Gold_Stash:
            return 25;
        default:
            return 9;
        }
    }

    int propGet(void* inp, propId type) {
        return propGet(inp, type,propFind(inp,type));
    }

    void propSet(void* inp, propId type, int value) {
        propSet(inp, type, value, propFind(inp,type));
    }

    void propSet(void* inp, propId type, quint32 value, int shift) {
        quint64 *i;
        quint64 j=value;
        i = ((quint64*)(inp+shift/8));
        printf("%llx\n",*i);
        j=j<<(shift%8);
        quint64 mask=(1ULL<<propLen(type))-1;
        mask=mask<<(shift%8);
        *i=*i&(~mask);
        *i=*i|(j&mask);
    }

    quint32 propGet(void* inp, propId type, int shift) {
        quint64 i=0;
        i = *((long long int*)(inp+shift/8));
        i=i>>(shift%8);
        quint64 mask=(1ULL<<propLen(type))-1;
        return i&mask;
    }

    int propFind(void* inp, propId type_) {
        int shift=0;
        int size=propSize((char*)inp);
        while(shift>>3<size) {
            propId type = (propId)propGet(inp, (propId)-1, shift);
            shift+=propLen((propId)-1);
            if(type==type_)
                break;
            else
                shift+=propLen(type);
        }
        return shift;
    }

    int propSize(char* inp) {
        int size=0;
        while(inp[0]!='i' && inp[1]!='f') {
            size++;
            inp++;
        }
        return size;
    }

    void checksum() {
        header()->dwCRC=0;

        quint32 checksum=0;
        foreach(quint8 data, data_)
            checksum =(checksum << 1 | checksum >> 31)+data;
        header()->dwCRC=checksum;
    }

public:
    d2s(QString filename) {
        QFile f(filename);
        f.open(QIODevice::ReadOnly);
        data_=f.readAll();
        f.close();
    }

    d2Header* header() {
        return (d2Header*)data_.data();
    }

    d2Woo* woo() {
        int offset=sizeof(d2Header);
        return (d2Woo*)(data_.data()+offset);
    }

    d2Ws* wayp() {
        int offset=sizeof(d2Header)+woo()->wSize;
        return (d2Ws*)(data_.data()+offset);
    }

    d2W4* greetings() {
        int offset=sizeof(d2Header)+woo()->wSize+sizeof(d2Ws);
        return (d2W4*)(data_.data()+offset);
    }

    int prop(propId type) {
        int offset=sizeof(d2Header)+woo()->wSize+sizeof(d2Ws)+sizeof(d2W4);
        return propGet(data_.data()+offset+2, type);
    }

    int setProp(propId type, int value) {
        int offset=sizeof(d2Header)+woo()->wSize+sizeof(d2Ws)+sizeof(d2W4);
        propSet(data_.data()+offset+2, type, value);
    }

    void save(QString filename) {
        checksum();
        QFile f(filename);
        f.open(QIODevice::WriteOnly);
        f.write(data_);
        f.close();
    }
};

int main(int argc, char *argv[])
{
    QFile::copy(argv[1],QString(argv[1])+".bak");
    d2s save(argv[1]);
    if(QString(argv[2])=="reset")
    {
        save.woo()->acts[5].wQuestStatus[0]=0x2002;
        qDebug()<<"Akara skill/stat reset enabled";
    }
    else if(QString(argv[2])=="imbue") {
        save.woo()->acts[0].wQuestStatus[2]|=0x0002;
        save.woo()->acts[0].wQuestStatus[2]&=0xfffe;
        qDebug()<<"Imbue enabled";
    }
    else if(QString(argv[2])=="socket") {
        save.woo()->acts[4].wQuestStatus[2]|=0x0002;
        save.woo()->acts[4].wQuestStatus[2]&=0xfffe;
        qDebug()<<"Socket enabled";
    }
    else if(QString(argv[2])=="list") {
        QString val(argv[3]);
        int a=val.toInt();
        printf("%x\n",save.woo()->acts[a].wActStart);
        for(int i=0; i<6; ++i)
            printf("%x\n",save.woo()->acts[a].wQuestStatus[i]);
        printf("%x\n",save.woo()->acts[a].wActEnd);
    }
    else if(QString(argv[2])=="addExp") {
        int xp=save.prop(d2s::Experience);
        int i=QString(argv[3]).toInt();
        save.setProp(d2s::Experience,xp+i);
        qDebug()<<"XP set to "<<save.prop(d2s::Experience);
    }
    else if(QString(argv[2])=="addGold") {
        d2s::propId type=d2s::Gold_Stash;
        int gold=save.prop(type);
        if(gold==0){
            type=d2s::Gold;
            gold=save.prop(type);
        }
        QString val(argv[3]);
        int i=val.toInt();
        save.setProp(type,gold+i);
        qDebug()<<"Gold set to "<<save.prop(type);
    } else {
        qDebug()<<"Invalid action "<<argv[2];
        return 2;
    }
    save.save(argv[1]);
}
