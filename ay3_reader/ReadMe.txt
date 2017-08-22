
AY-file specifications.
-----------------------

This is text from readme.txt of Micro Speccy (an AY-files player)

- AY-file format (thanks to Patrik Rak for help).

  Originally this format was supporting on Amiga's DeliTracker (DeliAY plug-in).
  Therefore, all word data is in Motorola order (from hi to low). Also, all
  pointers are signed and relative (from current position in file to pointed
  data). So, AY-file is sequences of records. First record is header. Here you
  are:

  TAYFileHeader=packed record
   FileID,			//'ZXAY'
   TypeID:longword;		//'EMUL'
   FileVersion,			//Version of song file (release version)
   PlayerVersion:byte;		//Required player version. Micro Speccy
				//works as player of version 3.
				//Now available values:
				//0       Use zero, if you do not
				//        know what player version
				//        you need
				//All other player versions and short
				//description of these players next:
				//1       initial player version
				//2       first 256 bytes is
				//        filled with 0xC9 (ret),
				//        not just last init byte
				//3       PC rewrite, different
				//        Z80 init routine, full
				//        Z80 emulation, supports
				//        48k tunes.
   PSpecialPlayer,		//It's for tunes which contained "custom
				//player" in m68k assembly. As Patrik Rak
				//is saying, exists only one AY-file of this
				//kind. So, can be simply ignored.
   PAuthor,PMisc:smallint;	//Relative pointers to the
				//Author and Misc NTStrings
   NumOfSongs,FirstSong:byte;	//Number of songs and first song values, both
				//are decreased by 1
   PSongsStructure:smallint;	//Relative pointer to song structure
  end;

  Next is song structure (repeated NumOfSongs + 1 times):

  TSongStructure=packed record
   PSongName,PSongData:smallint;//Relative pointers to NTStr song
				//name and song data
   end;

  All described above is rightly for any AY-file. Following data only for
  TypeID = 'EMUL'. Next is song data:
 
  TSongData=packed record
   AChan,BChan,
   CChan,Noise:byte;		//These are actually four bytes which specify
				//which Amiga channels will play which AY
				//channel (A,B,C and Noise, respectively). The
				//legal value is any permutation of 0,1,2,3.
				//The most common is 0,1,2,3. Not used in
				//Micro Speccy, of course
   SongLength:word;		//Song duration in 1/50 of second. If ZERO then
				//length is unknown (infinitely). Not used in
				//Micro Speccy
   FadeLength:word;		//Duration of volume fade after end of song in
				//1/50 of second. Not used in Micro Speccy
   HiReg,LoReg:byte;		//HiReg and LoReg for all common registers:
				//AF,AF',HL,HL',DE,DE',BC,BC',IX and IY
   PPoints,			//Relative pointer to points data
   PAddresses:smallint;		//Relative pointer to blocks data
  end;

  Next is points data:

  TPoints = packed record
   Stack,Init,Inter:word;	//Values for SP, INIT and INTERRUPT addresses
  end;

  Next is blocks data (words, offsets are signed):

      Address1			//Address of 1st block in Z80 memory
      Length1			//Length of 1st block
      Offset1			//Relative offset in AY-file to this block
      Address2                  //and for 2nd block
      Length2
      Offset2
      ...
      AddressN			//and for Nth (last) block
      LengthN
      OffsetN
      ENDWORD                   //ZERO word (end of blocks data)
				//so block can't have ZERO
				//address

  Notes from Patrik Rak:
  The word values should be always aligned at 2 byte offsets. Otherwise the
  DeliAY will crash (it's the limitation of the m68000).

  In case Address+Length > 65536, DeliAY decreases the size to make it
  == 65536.

  From me:
  In case CurrPositionInAYFile+Offset+Length > AYFileSize, MicroSpeccy
  and AYPlay decreases the size to make it == AYFileSize. I must to say,
  that if it is then this is broken AY-file. But there are many that ones
  in the world.

- How player of version 3 must play one of songs of AY File

    a) Fill #0000-#00FF range with #C9 value
    b) Fill #0100-#3FFF range with #FF value
    c) Fill #4000-#FFFF range with #00 value
    d) Place to #0038 address #FB value
    e) if INIT equal to ZERO then place to first CALL instruction address of
       first AY file block instead of INIT (see next f) and g) steps)
    f) if INTERRUPT equal to ZERO then place at ZERO address next player:

		DI
		CALL    INIT
	LOOP:	IM      2
		EI
		HALT
		JR      LOOP

    g) if INTERRUPT not equal to ZERO then place at ZERO address next player:

		DI
		CALL    INIT
	LOOP:	IM      1
		EI
		HALT
		CALL INTERRUPT
		JR      LOOP

    h) Load all blocks for this song
    i) Load all common lower registers with LoReg value (including AF
       register)
    j) Load all common higher registers with HiReg value
    k) Load into I register 3 (this player version)
    l) load to SP stack value from points data of this song
    m) Load to PC ZERO value
    n) Disable Z80 interrupts and set IM0 mode
    o) Emulate resetting of AY chip
    p) Start Z80 emulation

  As you can see, blocks of AY files can to rewrite standard player routine
  with own one. So, if you can not adapt some of tunes to standard player,
  you can write own player and place it at INIT address or even at 0x0001
  address (block of AY-file can be loaded at any address except 0x0000).

Links.
------

- AYMakeR supporting site
  http://bulba.at.kz/
- AYMakeR author
  Sergey Bulba, vorobey@mail.khstu.ru
- .AY files and AYmake V0.11 by James McKay (24/05/2000)
  http://www.void.jump.org/projectay
- Amstrad CPC .AY files (1.3 Mb archive)
  http://cracktros.planet-d.net/cpc/emulcpc.zip
- Project AY site developer
  bcass, bcass01@hotmail.com

Source text.
------------

AYMakeR comes together with its source text in Pascal. You can use it as free
as you want.

Understanding AY-files ideology.
--------------------------------

For understanding AY-files ideology, I am recommending to split several already
existing AY-files. I had wrote special tool called "aysplitr". You can download
it from http://bulba.at.kz/. There are also old "aysplit" for old James McKay's
AYMake.
