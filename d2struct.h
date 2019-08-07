#ifndef D2STRUCT_H
#define D2STRUCT_H
#define DWRD unsigned int
#define WORD unsigned short int
#define BYTE unsigned char
#pragma pack(push,1)
struct d2W4 {
    WORD wMagic;//0x7701
    WORD wSize;
    DWRD normal[2];
    DWRD nightmare[2];
    DWRD hell[2];
    DWRD normal1[2];
    DWRD nightmare1[2];
    DWRD hell1[2];
};
struct d2Ws {
    char wMagic[2];
    BYTE unk[6];
    DWRD wp[3*6];
};
struct d2Act {
    WORD wActStart;
    WORD wQuestStatus[6];
    WORD wActEnd;
};
struct d2Woo {
    char dwMagic[4];//Woo!
    DWRD dwActs;//32 num of acts
    WORD wSize;//size in bytes
    d2Act acts[];//3*dwActs
};
struct d2Header {
    char dwMajic[4]; //	File ID	 	 	 	 	 	 	 +0
    DWRD dwVersion;//File Version	 	 	 	 	 	 	 +4
    DWRD dwSize;//File Size	 	 	 	 	 	 	 +8
    DWRD dwCRC;//File CRC	 	 	 	 	 	 	 +c
    DWRD dwWeaponSet;//Weapon Set	 	 	 	 	 	 	 +10
    char Name[16];//Character Name	 	 	 	 	 	 	 +14
    BYTE charType;//Character Type	 	 	 	 	 	 	 +24 // 0x40 = ladder 0x20 = expansion 0x10 =? 0x08 = HasDied 0x04 = Hardcore 0x02 = ? 0x01 = new character format
    BYTE charTitle;//Character Title	 	 	 	 	 	 	 +25
    WORD unk1;//Unknown	 	 	 	 	 	 	 +26 // 00 00
    BYTE charClass;//Character Class	 	 	 	 	 	 	 +28
    WORD unk2;//	16	Unknown	 	 	 	 	 	 	 +29 // 10 1E
    BYTE charLevel;//	8	Level	 	 	 	 	 	 	 +2b
    DWRD unk3;//	32	Unknown	 	 	 	 	 	 	 +2c
    DWRD dwTime;//	32	Time Stamp	 	 	 	 	 	 	 +30
    DWRD unk4;//	32	Unknown	 	 	 	 	 	 	// FF FF FF FF +34
    DWRD dwSkillKey[16];//	32	Skill Keys	 	 	 	 	 	 	// No Array support in format !!! +38
    DWRD dwLeftSkill1;//	32	Left Skill Weapon Set 1	 	 	 	 	 	 	 +78
    DWRD dwRightSkill1;//	32	Right Skill Weapon Set 1	 	 	 	 	 	 	 +7c
    DWRD dwLeftSkill2;//	32	Left Skill Weapon Set 2	 	 	 	 	 	 	 +80
    DWRD dwRightSkill2;//	32	Right Skill Weapon Set 2	 	 	 	 	 	 	 +84
    BYTE outfit[16];//	8	Character Load graphics	 	 	 	 	 	 	// No Array support in format !!! +88
    BYTE colors[16];//	8	Character Load Colors	 	 	 	 	 	 	// No Array support in format !!! +98
    BYTE Town1;//	8	Normal Town	 	 	 	 	 	 	// Only town for last played mode is set
    BYTE Town2;//	8	Nightmare Town
    BYTE Town3;//	8	Hell Town
    DWRD dwMapSeed;//	32	Map Random Seed
    WORD unk5;//	16	Unknown	 	 	 	 	 	 	// 00 00
    BYTE bMercDead;//	8	Merc Dead Flag
    BYTE unk6;//	8	Unknown
    DWRD dwMercControl;//	32	Merc Control seed
    WORD wMercName;//	16	Merc Name Index
    WORD wMercType;//	16	Merc Type
    DWRD dwMercExp;//	32	Merc Experience
    BYTE unk7[0x90];//	8	Unknown	 	 	 	 	 	 	// No Array support in format !!!
};
#pragma pack(pop)
#endif // D2STRUCT_H
