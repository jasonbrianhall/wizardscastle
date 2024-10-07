DECLARE FUNCTION CalculateCoordinate% (Level%)
DECLARE FUNCTION CalculateDimension% (Level%)
DECLARE FUNCTION AdjustLargeNumber% (InputNumber%)

1000 REM *****************************************************
1005 REM *                                                   *
1010 REM * WIZARD'S CASTLE GAME FROM JULY/AUGUST 1980        *
1015 REM * ISSUE OF RECREATIONAL COMPUTING MAGAZINE          *
1020 REM * WRITTEN FOR EXIDY SORCERER BY JOSEPH R. POWER     *
1025 REM * MODIFIED FOR HEATH MICROSOFT BASIC BY J.F.STETSON *
1026 REM * MODIFIED FOR QB64PE BY STEVE(TM)!!                *
1030 REM *                                                   *
1035 REM *****************************************************
1040 DEFINT A-Z
1045 DIM RoomContents$(34), RoomSymbol$(34), Race$(4), WeaponName$(8), EdiblePart$(8)
1050 DIM LocationMap(512), CurseEffect(3, 4), Treasure(8), OrbLocation(3), RunestaffLocation(3)
1080 YesNoPrompt$ = "** PLEASE ANSWER YES OR NO"
1085 GameNumber = 0
1090 REM
1095 REM   INITIALIZE ARRAYS
1100 REM
1105 GameNumber = GameNumber + 1
1110 RandomSeed = RND(1)
1115 RESTORE
1120 FOR Index = 1 TO 34
1125 READ RoomContents$(Index), RoomSymbol$(Index)
1130 NEXT Index
1135 FOR Index = 1 TO 512
1140 LocationMap(Index) = 101
1145 NEXT Index
1150 FOR Index = 1 TO 8
1155 READ WeaponName$(Index), EdiblePart$(Index)
1160 NEXT Index
1165 FOR Index = 1 TO 4
1170 READ Race$(Index)
1175 NEXT Index

IF GameNumber < 2 THEN
1190 PrintStars
1195 PRINT TAB(16); "* * * THE WIZARD'S CASTLE * * *"
1200 PRINT
1205 PrintStars
1210 PRINT "MANY CYCLES AGO, IN THE KINGDOM OF N'DIC, THE GNOMIC"
1215 PRINT "WIZARD ZOT FORGED HIS GREAT *ORB OF POWER*. HE SOON"
1220 PRINT "VANISHED, LEAVING BEHIND HIS VAST SUBTERRANEAN CASTLE"
1225 PRINT "FILLED WITH ESURIENT MONSTERS, FABULOUS TREASURES, AND"
1230 PRINT "THE INCREDIBLE *ORB OF ZOT*. FROM THAT TIME HENCE, MANY"
1235 PRINT "A BOLD YOUTH HAS VENTURED INTO THE WIZARD'S CASTLE. AS"
1240 PRINT "OF NOW, *NONE* HAS EVER EMERGED VICTORIOUSLY! BEWARE!!"
1245 PRINT
END IF

1250 PlayerX = 1: PlayerY = 4
1255 LocationMap(CalculateDimension(1)) = 2
1260 FOR CurrentLevel = 1 TO 7
1265 FOR StairCount = 1 TO 2
1270 RoomType = 104
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1280 LocationMap(CalculateDimension(CurrentLevel + 1)) = 103
1285 NEXT StairCount
1290 NEXT CurrentLevel
1295 FOR CurrentLevel = 1 TO 8
1300 FOR RoomType = 113 TO 124
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1310 NEXT RoomType
1315 FOR VendorCount = 1 TO 3
1320 FOR RoomType = 105 TO 112
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1330 NEXT RoomType
1335 RoomType = 125
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1345 NEXT VendorCount
1350 NEXT CurrentLevel
1355 FOR RoomType = 126 TO 133
1360 CurrentLevel = RandomNumber(8)
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1370 NEXT RoomType
1375 RoomType = 101
1380 FOR CurseIndex = 1 TO 3
1385 CurrentLevel = RandomNumber(8)
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1395 CurseEffect(CurseIndex, 1) = PlayerX
1400 CurseEffect(CurseIndex, 2) = PlayerY
1405 CurseEffect(CurseIndex, 3) = CurrentLevel
1410 CurseEffect(CurseIndex, 4) = 0
1415 NEXT CurseIndex
1420 PlayerRace = 0
1425 PlayerStrength = 2
1430 PlayerDexterity = 14
1435 Race$(3) = "MAN"
1440 RoomType = 112 + RandomNumber(12)
1445 CurrentLevel = RandomNumber(8)
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1455 RunestaffLocation(1) = PlayerX
1460 RunestaffLocation(2) = PlayerY
1465 RunestaffLocation(3) = CurrentLevel
1470 RoomType = 109
1475 CurrentLevel = RandomNumber(8)
PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WHILE LocationMap(CalculateDimension(CurrentLevel)) <> 101
    PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
WEND
LocationMap(CalculateDimension(CurrentLevel)) = RoomType
1485 OrbLocation(1) = PlayerX
1490 OrbLocation(2) = PlayerY
1495 OrbLocation(3) = CurrentLevel
1500 BookFlag = 0: OtherPoints = 8: ArmorValue = 0: HitPoints = 0: TurnCount = 1: VendorFlag = 0: LampFlag = 0
1505 TreasureCount = 0: GoldPieces = 60: RunestaffFlag = 0: OrbFlag = 0: BlindnessFlag = 0: PlayerIntelligence = 8: PlayerSex = 0
1510 FOR Index = 1 TO 8
1515 Treasure(Index) = 0
1520 NEXT Index
1525 PRINT CHR$(7);
1530 CALL ChooseRace(PlayerRace, PlayerStrength, PlayerDexterity, OtherPoints)
1585 CALL ChooseSex(PlayerSex, PlayerRace)
1620 PRINT "OK, "; Race$(PlayerRace); ", YOU HAVE THE FOLLOWING ATTRIBUTES :"
1625 PRINT "STRENGTH ="; PlayerStrength; " INTELLIGENCE ="; PlayerIntelligence; " DEXTERITY ="; PlayerDexterity
1630 PRINT "AND"; OtherPoints; "OTHER POINTS TO ALLOCATE AS YOU WISH."
1635 PRINT
1640 AttributeName$ = "STRENGTH"
DO
    PRINT "HOW MANY POINTS DO YOU WISH TO ADD TO YOUR "; AttributeName$;
    INPUT UserInput$
    PRINT
    PointsAllocated = VAL(UserInput$)
    IF PointsAllocated = 0 AND ASC(UserInput$) <> 48 THEN PointsAllocated = -1
    IF PointsAllocated < 0 OR PointsAllocated > OtherPoints OR PointsAllocated <> INT(PointsAllocated) THEN
        PRINT "** INVALID INPUT. PLEASE TRY AGAIN."
    ELSE
        OtherPoints = OtherPoints - PointsAllocated
        EXIT DO
    END IF
LOOP
1650 PlayerStrength = PlayerStrength + PointsAllocated
IF OtherPoints > 0 THEN
    AttributeName$ = "INTELLIGENCE"
    DO
        PRINT "HOW MANY POINTS DO YOU WISH TO ADD TO YOUR "; AttributeName$;
        INPUT UserInput$
        PRINT
        PointsAllocated = VAL(UserInput$)
        IF PointsAllocated = 0 AND ASC(UserInput$) <> 48 THEN PointsAllocated = -1
        IF PointsAllocated < 0 OR PointsAllocated > OtherPoints OR PointsAllocated <> INT(PointsAllocated) THEN
            PRINT "** INVALID INPUT. PLEASE TRY AGAIN."
        ELSE
            OtherPoints = OtherPoints - PointsAllocated
            EXIT DO
        END IF
    LOOP
    PlayerIntelligence = PlayerIntelligence + PointsAllocated

    IF OtherPoints > 0 THEN
        AttributeName$ = "DEXTERITY"
        DO
            PRINT "HOW MANY POINTS DO YOU WISH TO ADD TO YOUR "; AttributeName$;
            INPUT UserInput$
            PRINT
            PointsAllocated = VAL(UserInput$)
            IF PointsAllocated = 0 AND ASC(UserInput$) <> 48 THEN PointsAllocated = -1
            IF PointsAllocated < 0 OR PointsAllocated > OtherPoints OR PointsAllocated <> INT(PointsAllocated) THEN
                PRINT "** INVALID INPUT. PLEASE TRY AGAIN."
            ELSE
                OtherPoints = OtherPoints - PointsAllocated
                EXIT DO
            END IF
        LOOP
        PlayerDexterity = PlayerDexterity + PointsAllocated
    END IF
END IF

1695 PRINT "OK, "; Race$(PlayerRace); ", YOU HAVE 60 GOLD PIECES (GP'S)."
1700 EquipmentType$ = "ARMOR"
PRINT
PRINT "THESE ARE THE TYPES OF "; EquipmentType$; " YOU CAN BUY :"
1710 CALL ChooseArmor(ArmorValue, ArmorHitPoints, GoldPieces, PlayerRace)
1760 PRINT
1765 PRINT "OK, BOLD "; Race$(PlayerRace); ", YOU HAVE"; GoldPieces; "GP'S LEFT."
EquipmentType$ = "WEAPONS"
DO
    PRINT
    PRINT "THESE ARE THE TYPES OF "; EquipmentType$; " YOU CAN BUY :"
    PRINT "SWORD<30> MACE<20> DAGGER<10> NOTHING<0>"
    PRINT
    PRINT "YOUR CHOICE";
    INPUT UserInput$
    UserInput$ = UCASE$(LEFT$(UserInput$, 1))

    IF UserInput$ = "N" THEN
        WeaponValue = 0
        EXIT DO
    ELSE
        WeaponValue = 3 * (UserInput$ = "S") + 2 * (UserInput$ = "M") + (UserInput$ = "D")
        IF WeaponValue > 0 THEN
            EXIT DO
        ELSE
            PRINT
            PRINT "** IS YOUR IQ REALLY"; PlayerIntelligence; "?"
        END IF
    END IF
LOOP

GoldPieces = GoldPieces - WeaponValue * 10

IF GoldPieces >= 20 THEN
    DO
        PRINT
        PRINT "DO YOU WANT TO BUY A LAMP FOR 20 GP'S";
        PRINT
        PRINT "YOUR CHOICE";
        INPUT UserInput$
        UserInput$ = UCASE$(LEFT$(UserInput$, 1))

        IF UserInput$ = "Y" THEN
            LampFlag = 1
            GoldPieces = GoldPieces - 20
            EXIT DO
        ELSEIF UserInput$ = "N" THEN
            EXIT DO
        ELSE
            PRINT
            PRINT YesNoPrompt$
            PRINT
        END IF
    LOOP
END IF

PRINT

FlareCount = 0
IF GoldPieces >= 1 THEN 
1865 PRINT "OK, "; Race$(PlayerRace); ", YOU HAVE"; GoldPieces; "GOLD PIECES LEFT."
1870 PRINT
1875 CALL BuyFlares(FlareCount, GoldPieces)
END IF
1920 PlayerX = 1: PlayerY = 4: CurrentLevel = 1
1925 PRINT "OK, "; Race$(PlayerRace); ", YOU ARE NOW ENTERING THE CASTLE!"
1930 GOTO 3450
1935 REM
1940 REM   MAIN PROCESSING LOOP
1945 REM
1950 TurnCount = TurnCount + 1
IF RunestaffFlag + OrbFlag = 0 THEN
    IF CurseEffect(1, 4) > Treasure(1) THEN TurnCount = TurnCount + 1
    IF CurseEffect(2, 4) > Treasure(3) THEN 
        GoldPieces = GoldPieces - RandomNumber(5)
        IF GoldPieces < 0 THEN GoldPieces = 0
    END IF
    IF CurseEffect(3, 4) > Treasure(5) THEN
        TempX = PlayerX: TempY = PlayerY: TempLevel = CurrentLevel
        PlayerX = RandomNumber(8): PlayerY = RandomNumber(8): CurrentLevel = RandomNumber(8)
        LocationMap(CalculateDimension(CurrentLevel)) = AdjustLargeNumber(LocationMap(CalculateDimension(CurrentLevel))) + 100
        PlayerX = TempX: PlayerY = TempY: CurrentLevel = TempLevel
        IF LocationMap(CalculateDimension(CurrentLevel)) = 1 THEN
            FOR CurseIndex = 1 TO 3
                CurseEffect(CurseIndex, 4) = -(CurseEffect(CurseIndex, 1) = PlayerX) * (CurseEffect(CurseIndex, 2) = PlayerY) * (CurseEffect(CurseIndex, 3) = CurrentLevel)
            NEXT CurseIndex
        END IF
    END IF
END IF

IF RandomNumber(5) = 1 THEN
    PRINT
    PRINT "YOU ";
    EventType = RandomNumber(7) + BlindnessFlag
    IF EventType > 7 THEN EventType = 4
    SELECT CASE EventType
        CASE 1
            PRINT "SEE A BAT FLY BY!"
        CASE 2
            PRINT "HEAR ";
            SoundType = RandomNumber(4)
            SELECT CASE SoundType
                CASE 1: PRINT "A SCREAM!"
                CASE 2: PRINT "FOOTSTEPS!"
                CASE 3: PRINT "A WUMPUS!"
                CASE 4: PRINT "THUNDER!"
            END SELECT
        CASE 3: PRINT "SNEEZED!"
        CASE 4: PRINT "STEPPED ON A FROG!"
        CASE 5: PRINT "SMELL "; RoomContents$(12 + RandomNumber(13)); " FRYING!"
        CASE 6: PRINT "FEEL LIKE YOU'RE BEING WATCHED!"
        CASE 7: PRINT "HEAR FAINT RUSTLING NOISES!"
    END SELECT
END IF

IF BlindnessFlag + Treasure(4) = 2 THEN
    PRINT
    PRINT RoomContents$(29); " CURES YOUR BLINDNESS!"
    BlindnessFlag = 0
END IF

IF BookFlag + Treasure(6) = 2 THEN
    PRINT
    PRINT RoomContents$(31); " DISSOLVES THE BOOK!"
    BookFlag = 0
END IF

PRINT

2210 LINE INPUT "ENTER YOUR COMMAND : "; UserCommand$
2215 IF LEFT$(UserCommand$, 2) = "DR" GOTO 2870
2220 UserCommand$ = LEFT$(UserCommand$, 1)
2225 IF UserCommand$ = "N" GOTO 2435
2230 IF (UserCommand$ = "S") OR (UserCommand$ = "W") OR (UserCommand$ = "E") GOTO 2440
2235 IF UserCommand$ = "U" GOTO 2465
2240 IF UserCommand$ = "D" GOTO 2480
2245 IF UserCommand$ = "M" GOTO 2505
2250 IF UserCommand$ = "F" THEN ON BlindnessFlag + 1 GOTO 2620, 2505
2255 IF UserCommand$ = "L" THEN ON BlindnessFlag + 1 GOTO 2750, 2505
2260 IF UserCommand$ = "O" GOTO 2965
2265 IF UserCommand$ = "G" THEN ON BlindnessFlag + 1 GOTO 3185, 2505
2270 IF UserCommand$ = "T" THEN PRINT : ON RunestaffFlag + 1 GOTO 3315, 3335
2275 IF UserCommand$ = "Q" GOTO 3390
2280 IF UserCommand$ <> "H" GOTO 2420
2285 PRINT CHR$(27); "E"
2290 PRINT "*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***"
2295 PRINT
2300 PRINT "THE FOLLOWING COMMANDS ARE AVAILABLE :"
2305 PRINT
2310 PRINT "H/ELP     N/ORTH    S/OUTH    E/AST     W/EST     U/P"
2315 PRINT "D/OWN     DR/INK    M/AP      F/LARE    L/AMP     O/PEN"
2320 PRINT "G/AZE     T/ELEPORT Q/UIT"
2325 PRINT
2330 PRINT "THE CONTENTS OF ROOMS ARE AS FOLLOWS :"
2335 PRINT
2340 PRINT ". = EMPTY ROOM      B = BOOK            C = CHEST"
2345 PRINT "D = STAIRS DOWN     E = ENTRANCE/EXIT   F = FLARES"
2350 PRINT "G = GOLD PIECES     M = MONSTER         O = CRYSTAL ORB"
2355 PRINT "P = MAGIC POOL      S = SINKHOLE        T = TREASURE"
2360 PRINT "U = STAIRS UP       V = VENDOR          W = WARP/ORB"
2365 PRINT
2370 PRINT "THE BENEFITS OF HAVING TREASURES ARE :"
2375 PRINT
2380 PRINT "RUBY RED - AVOID LETHARGY     PALE PEARL - AVOID LEECH"
2385 PRINT "GREEN GEM - AVOID FORGETTING  OPAL EYE - CURES BLINDNESS"
2390 PRINT "BLUE FLAME - DISSOLVES BOOKS  NORN STONE - NO BENEFIT"
2395 PRINT "PALANTIR - NO BENEFIT         SILMARIL - NO BENEFIT"
2400 PRINT
2405 PRINT "PRESS RETURN WHEN READY TO RESUME, "; Race$(PlayerRace); ".";
2410 LINE INPUT ""; UserInput$
2415 GOTO 1950
2420 PRINT
2425 PRINT "** SILLY "; Race$(PlayerRace); ", THAT WASN'T A VALID COMMAND!"
2430 GOTO 1950
2435 IF LocationMap(CalculateDimension(CurrentLevel)) = 2 GOTO 4970
2440 PlayerX = PlayerX + (UserCommand$ = "N") - (UserCommand$ = "S")
2445 PlayerY = PlayerY + (UserCommand$ = "W") - (UserCommand$ = "E")
2450 PlayerX = AdjustNumber(PlayerX)
2455 PlayerY = AdjustNumber(PlayerY)
2460 GOTO 3450
2465 IF LocationMap(CalculateDimension(CurrentLevel)) = 3 THEN CurrentLevel = CurrentLevel - 1: GOTO 3450
2470 DirectionName$ = "UP"
2475 GOTO 2490
2480 DirectionName$ = "DOWN"
2485 IF LocationMap(CalculateDimension(CurrentLevel)) = 4 THEN CurrentLevel = CurrentLevel + 1: GOTO 3450
2490 PRINT
2495 PRINT "** THERE ARE NO STAIRS GOING "; DirectionName$; " FROM HERE!"
2500 GOTO 1950
2505 IF BlindnessFlag <> 1 GOTO 2540
2510 PRINT
2515 PRINT "** YOU CAN'T SEE ANYTHING, YOU DUMB "; Race$(PlayerRace); "!"
2520 GOTO 1950
2525 REM
2530 REM   DISPLAY MAP OF CURRENT CASTLE LEVEL
2535 REM
2540 PRINT
2545 TempX = PlayerX: TempY = PlayerY
2550 FOR MapX = 1 TO 8
2555 FOR MapY = 1 TO 8
2560 RoomContent = LocationMap(CalculateDimension(CurrentLevel))
2565 IF RoomContent > 99 THEN RoomContent = 34         ' LET RoomContent=RoomContent-100 TO SHOW ROOMS
2570 IF MapX = TempX AND MapY = TempY THEN PRINT "<"; RoomSymbol$(RoomContent); ">  "; : GOTO 2580
2575 PRINT " "; RoomSymbol$(RoomContent); "   ";
2580 NEXT MapY
2585 PRINT
2590 PRINT
2595 NEXT MapX
2600 PlayerX = TempX: PlayerY = TempY
2605 GOTO 2725
2610 PRINT ") LEVEL"; CurrentLevel
2615 GOTO 1950
2620 IF FlareCount <> 0 GOTO 2650
2625 PRINT "** HEY, BRIGHT ONE, YOU'RE OUT OF FLARES!"
2630 GOTO 1950
2635 REM
2640 REM   DISPLAY ADJACENT ROOM CONTENTS WITH FLARE
2645 REM
2650 PRINT
2655 FlareCount = FlareCount - 1
2660 TempX = PlayerX: TempY = PlayerY
2665 FOR AdjacentX = TempX - 1 TO TempX + 1
2670 PlayerX = AdjustNumber(AdjacentX)
2675 FOR AdjacentY = TempY - 1 TO TempY + 1
2680 PlayerY = AdjustNumber(AdjacentY)
2685 RoomContent = AdjustLargeNumber(LocationMap(CalculateDimension(CurrentLevel)))
2690 LocationMap(CalculateDimension(CurrentLevel)) = RoomContent
2695 PRINT " "; RoomSymbol$(RoomContent); "   ";
2700 NEXT AdjacentY
2705 PRINT
2710 PRINT
2715 NEXT AdjacentX
2720 PlayerX = TempX: PlayerY = TempY
2725 PRINT "YOU ARE AT ("; PlayerX; ","; PlayerY; ") LEVEL"; CurrentLevel; "."
2730 GOTO 1950
2735 REM
2740 REM   DISPLAY CONTENTS OF ADJACENT ROOM WITH LAMP
2745 REM
2750 IF LampFlag <> 0 GOTO 2770
2755 PRINT
2760 PRINT "** YOU DON'T HAVE A LAMP, "; Race$(PlayerRace); "!"
2765 GOTO 1950
2770 PRINT
2775 PRINT "WHERE DO YOU WANT TO SHINE THE LAMP (N,S,E,W)";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
2785 TempX = PlayerX: TempY = PlayerY
2790 PlayerX = AdjustNumber(PlayerX + (UserInput$ = "N") - (UserInput$ = "S"))
2795 PlayerY = AdjustNumber(PlayerY + (UserInput$ = "W") - (UserInput$ = "E"))
2800 IF TempX - PlayerX + TempY - PlayerY <> 0 GOTO 2820
2805 PRINT
2810 PRINT "** THAT'S NOT A DIRECTION, "; Race$(PlayerRace); "!"
2815 GOTO 1950
2820 PRINT
2825 PRINT "THE LAMP SHINES INTO ("; PlayerX; ","; PlayerY; ") LEVEL"; CurrentLevel; "."
2830 PRINT
2835 LocationMap(CalculateDimension(CurrentLevel)) = AdjustLargeNumber(LocationMap(CalculateDimension(CurrentLevel)))
2840 PRINT "THERE YOU WILL FIND "; RoomContents$(LocationMap(CalculateDimension(CurrentLevel))); "."
2845 PlayerX = TempX: PlayerY = TempY
2850 GOTO 1950
2855 REM
2860 REM   TAKE A DRINK FROM A POOL
2865 REM
2870 IF LocationMap(CalculateDimension(CurrentLevel)) = 5 GOTO 2890
2875 PRINT
2880 PRINT "** IF YOU WANT A DRINK, FIND A POOL!"
2885 GOTO 1950
2890 DrinkEffect = RandomNumber(8)
2895 PRINT
2900 PRINT "YOU TAKE A DRINK AND ";
2905 IF DrinkEffect < 7 THEN PRINT "FEEL ";
2910 ON DrinkEffect GOTO 2915, 2920, 2925, 2930, 2935, 2940, 2945, 2955
2915 PlayerStrength = CalculateCoordinate(PlayerStrength + RandomNumber(3)): PRINT "STRONGER.": GOTO 1950
2920 PlayerStrength = PlayerStrength - RandomNumber(3): PRINT "WEAKER.": ON (1 - (PlayerStrength < 1)) GOTO 1950, 4910
2925 PlayerIntelligence = CalculateCoordinate(PlayerIntelligence + RandomNumber(3)): PRINT "SMARTER.": GOTO 1950
2930 PlayerIntelligence = PlayerIntelligence - RandomNumber(3): PRINT "DUMBER.": ON (1 - (PlayerIntelligence < 1)) GOTO 1950, 4910
2935 PlayerDexterity = CalculateCoordinate(PlayerDexterity + RandomNumber(3)): PRINT "NIMBLER.": GOTO 1950
2940 PlayerDexterity = PlayerDexterity - RandomNumber(3): PRINT "CLUMSIER.": ON (1 - (PlayerDexterity < 1)) GOTO 1950, 4910
2945 NewRace = RandomNumber(4): IF NewRace = PlayerRace GOTO 2945
2950 PlayerRace = NewRace: PRINT "BECOME A "; Race$(PlayerRace); ".": GOTO 1950
2955 PlayerSex = 1 - PlayerSex: PRINT "TURN INTO A "; : IF PlayerSex = 0 THEN PRINT "FE";
2960 PRINT "MALE "; Race$(PlayerRace); "!": GOTO 1950
2965 IF LocationMap(CalculateDimension(CurrentLevel)) <> 6 GOTO 2985
2970 PRINT
2975 PRINT "YOU OPEN THE CHEST AND"
2980 GOTO 3115
2985 IF LocationMap(CalculateDimension(CurrentLevel)) <> 12 GOTO 3005
2990 PRINT
2995 PRINT "YOU OPEN THE BOOK AND"
3000 GOTO 3020
3005 PRINT
3010 PRINT "** THE ONLY THING OPENED WAS YOUR BIG MOUTH!"
3015 GOTO 1950
3020 ON RandomNumber(6) GOTO 3025, 3040, 3050, 3060, 3075, 3090
3025 PRINT "FLASH! OH NO! YOU ARE NOW A BLIND "; Race$(PlayerRace); "!"
3030 BlindnessFlag = 1
3035 GOTO 3105
3040 PRINT "IT'S ANOTHER VOLUME OF ZOT'S POETRY! - YECH!!"
3045 GOTO 3105
3050 PRINT "IT'S AN OLD COPY OF PLAY"; Race$(RandomNumber(4)); "!"
3055 GOTO 3105
3060 PRINT "IT'S A MANUAL OF DEXTERITY!"
3065 PlayerDexterity = 18
3070 GOTO 3105
3075 PRINT "IT'S A MANUAL OF STRENGTH!"
3080 PlayerStrength = 18
3085 GOTO 3105
3090 PRINT "THE BOOK STICKS TO YOUR HANDS -"
3095 PRINT "NOW YOU ARE UNABLE TO DRAW YOUR WEAPON!"
3100 BookFlag = 1
3105 LocationMap(CalculateDimension(CurrentLevel)) = 1
3110 GOTO 1950
3115 ON RandomNumber(4) GOTO 3120, 3140, 3160, 3140
3120 PRINT "KABOOM! IT EXPLODES!!"
3125 DamagePoints = RandomNumber(6)
IF ArmorValue > 0 THEN
    DamagePoints = DamagePoints - ArmorValue
    ArmorHitPoints = ArmorHitPoints - ArmorValue
    IF DamagePoints < 0 THEN
        ArmorHitPoints = ArmorHitPoints - DamagePoints
        DamagePoints = 0
    END IF
    IF ArmorHitPoints < 0 THEN
        ArmorHitPoints = 0
        ArmorValue = 0
        PRINT
        PRINT "YOUR ARMOR HAS BEEN DESTROYED . . . GOOD LUCK!"
    END IF
END IF
PlayerStrength = PlayerStrength - DamagePoints
3135 ON (1 - (PlayerStrength < 1)) GOTO 3105, 4910
3140 GoldFound = RandomNumber(1000)
3145 PRINT "FIND"; GoldFound; "GOLD PIECES!"
3150 GoldPieces = GoldPieces + GoldFound
3155 GOTO 3105
3160 PRINT "GAS!! YOU STAGGER FROM THE ROOM!"
3165 LocationMap(CalculateDimension(CurrentLevel)) = 1
3170 TurnCount = TurnCount + 20
3175 UserCommand$ = MID$("NSEW", RandomNumber(4), 1)
3180 GOTO 2440
3185 IF LocationMap(CalculateDimension(CurrentLevel)) = 11 GOTO 3205
3190 PRINT
3195 PRINT "** IT'S HARD TO GAZE WITHOUT AN ORB!"
3200 GOTO 1950
3205 PRINT
3210 PRINT "YOU SEE ";
3215 ON RandomNumber(6) GOTO 3220, 3230, 3240, 3250, 3285, 3305
3220 PRINT "YOURSELF IN A BLOODY HEAP!"
3225 PlayerStrength = PlayerStrength - RandomNumber(2): ON (1 - (PlayerStrength < 1)) GOTO 1950, 4910
3230 PRINT "YOURSELF DRINKING FROM A POOL AND BECOMING "; RoomContents$(12 + RandomNumber(13)); "!"
3235 GOTO 1950
3240 PRINT RoomContents$(12 + RandomNumber(13)); " GAZING BACK AT YOU!"
3245 GOTO 1950
3250 TempX = PlayerX: TempY = PlayerY: TempLevel = CurrentLevel
3255 PlayerX = RandomNumber(8): PlayerY = RandomNumber(8): CurrentLevel = RandomNumber(8)
3260 RoomContent = AdjustLargeNumber(LocationMap(CalculateDimension(CurrentLevel)))
3265 LocationMap(CalculateDimension(CurrentLevel)) = RoomContent
3270 PRINT RoomContents$(RoomContent); " AT ("; PlayerX; ","; PlayerY; ") LEVEL"; CurrentLevel; "."
3275 PlayerX = TempX: PlayerY = TempY: CurrentLevel = TempLevel
3280 GOTO 1950
3285 OrbX = RandomNumber(8): OrbY = RandomNumber(8): OrbLevel = RandomNumber(8)
3290 IF RandomNumber(8) < 4 THEN OrbX = OrbLocation(1): OrbY = OrbLocation(2): OrbLevel = OrbLocation(3)
3295 PRINT "***THE ORB OF ZOT*** AT ("; OrbX; ","; OrbY; ") LEVEL"; OrbLevel; "!"
3300 GOTO 1950
3305 PRINT "A SOAP OPERA RERUN!"
3310 GOTO 1950
3315 IF RunestaffFlag <> 0 GOTO 3335
3320 PRINT
3325 PRINT "** YOU CAN'T TELEPORT WITHOUT THE RUNESTAFF!"
3330 GOTO 1950
3335 CoordinateName$ = "X-COORDINATE"
DO
    PRINT
    PRINT CoordinateName$;
    INPUT UserInput$
    EnteredCoordinate = INT(VAL(UserInput$))
    IF EnteredCoordinate > 0 AND EnteredCoordinate < 9 THEN
        EXIT DO
    ELSE
        PRINT
        PRINT "** TRY A NUMBER FROM 1 TO 8."
    END IF
LOOP
3345 PlayerX = EnteredCoordinate
3350 CoordinateName$ = "Y-COORDINATE"
DO
    PRINT
    PRINT CoordinateName$;
    INPUT UserInput$
    EnteredCoordinate = INT(VAL(UserInput$))
    IF EnteredCoordinate > 0 AND EnteredCoordinate < 9 THEN
        EXIT DO
    ELSE
        PRINT
        PRINT "** TRY A NUMBER FROM 1 TO 8."
    END IF
LOOP
3360 PlayerY = EnteredCoordinate
3365 CoordinateName$ = "Z-COORDINATE"
DO
    PRINT
    PRINT CoordinateName$;
    INPUT UserInput$
    EnteredCoordinate = INT(VAL(UserInput$))
    IF EnteredCoordinate > 0 AND EnteredCoordinate < 9 THEN
        EXIT DO
    ELSE
        PRINT
        PRINT "** TRY A NUMBER FROM 1 TO 8."
    END IF
LOOP
3375 CurrentLevel = EnteredCoordinate
3380 UserCommand$ = "T"
3385 GOTO 3450
3390 PRINT
3395 PRINT "DO YOU REALLY WANT TO QUIT NOW";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3405 PRINT
3410 IF UserInput$ = "Y" GOTO 3425
3415 PRINT "** THEN DON'T SAY THAT YOU DO!"
3420 GOTO 1950
3425 PRINT
3430 GOTO 5030
3435 REM
3440 REM   DISPLAY STATUS INFORMATION
3445 REM
3450 PRINT
PRINT "YOU ARE AT ("; PlayerX; ","; PlayerY; ") LEVEL"; CurrentLevel; "."
3460 PRINT "STRENGTH ="; PlayerStrength; " INTELLIGENCE ="; PlayerIntelligence; " DEXTERITY ="; PlayerDexterity
3465 PRINT "TREASURES ="; TreasureCount; " FLARES ="; FlareCount; " GOLD PIECES ="; GoldPieces
3470 PRINT "WEAPON = "; WeaponName$(WeaponValue + 1); "  ARMOR = "; WeaponName$(ArmorValue + 5);
3475 IF LampFlag = 1 THEN PRINT "  AND A LAMP";
3480 PRINT
3485 WebCount = 0
3490 RoomContent = AdjustLargeNumber(LocationMap(CalculateDimension(CurrentLevel)))
3495 LocationMap(CalculateDimension(CurrentLevel)) = RoomContent
3500 StatusMessage$ = "YOU NOW HAVE"
3505 PRINT
3510 PRINT "HERE YOU FIND "; RoomContents$(RoomContent); "."
3515 IF (RoomContent < 7) OR (RoomContent = 11) OR (RoomContent = 12) GOTO 1950
3520 IF RoomContent = 7 THEN GoldPieces = GoldPieces + RandomNumber(10): PRINT StatusMessage$; GoldPieces; ".": GOTO 3105
3525 IF RoomContent = 8 THEN FlareCount = FlareCount + RandomNumber(5): PRINT StatusMessage$; FlareCount; ".": GOTO 3105
3530 IF RoomContent > 9 GOTO 3545
3535 IF (OrbLocation(1) = PlayerX) AND (OrbLocation(2) = PlayerY) AND (OrbLocation(3) = CurrentLevel) THEN ON (1 - (UserCommand$ = "T")) GOTO 2440, 5175
3540 PlayerX = RandomNumber(8): PlayerY = RandomNumber(8): CurrentLevel = RandomNumber(8): GOTO 3450
3545 IF RoomContent = 10 THEN CurrentLevel = AdjustNumber(CurrentLevel + 1): GOTO 3450
3550 IF RoomContent <= 25 OR RoomContent >= 34 GOTO 3580
3555 PRINT
3560 PRINT "IT'S NOW YOURS!"
3565 Treasure(RoomContent - 25) = 1
3570 TreasureCount = TreasureCount + 1
3575 GOTO 3105
3580 MonsterType = LocationMap(CalculateDimension(CurrentLevel)) - 12
3585 WebCount = 0
3590 IF (MonsterType < 13) OR (VendorFlag = 1) GOTO 4185
3595 PRINT
3600 PRINT "YOU MAY TRADE WITH, ATTACK, OR IGNORE THE VENDOR."
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3610 IF UserInput$ = "I" GOTO 1950
3615 IF UserInput$ <> "A" GOTO 3640
3620 VendorFlag = 1
3625 PRINT
3630 PRINT "YOU'LL BE SORRY THAT YOU DID THAT!"
3635 GOTO 4185
3640 IF UserInput$ = "T" GOTO 3660
3645 PRINT
3650 PRINT "** NICE SHOT, "; Race$(PlayerRace); "!"
3655 GOTO 3595
3660 FOR TreasureIndex = 1 TO 8
3665 TreasureValue = RandomNumber(TreasureIndex * 1500)
3670 IF Treasure(TreasureIndex) = 0 GOTO 3700
3675 PRINT
3680 PRINT "DO YOU WANT TO SELL "; RoomContents$(TreasureIndex + 25); " FOR"; TreasureValue; "GP'S";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3690 IF UserInput$ = "Y" THEN TreasureCount = TreasureCount - 1: Treasure(TreasureIndex) = 0: GoldPieces = GoldPieces + TreasureValue: GOTO 3700
3695 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 3675
3700 NEXT TreasureIndex
3705 IF GoldPieces >= 1000 GOTO 3725
3710 PRINT
3715 PRINT "YOU'RE TOO POOR TO TRADE, "; Race$(PlayerRace); "."
3720 GOTO 1950
3725 IF GoldPieces < 1250 GOTO 3975
3730 PRINT
3735 PRINT "OK, "; Race$(PlayerRace); ", YOU HAVE"; GoldPieces; "GP'S AND "; WeaponName$(ArmorValue + 5); " ARMOR."
3740 PRINT
3745 EquipmentType$ = "ARMOR"
PRINT
PRINT "THESE ARE THE TYPES OF "; EquipmentType$; " YOU CAN BUY :"
3755 PRINT "NOTHING<0> LEATHER<1250> ";
3760 IF GoldPieces > 1499 THEN PRINT "CHAINMAIL<1500> ";
3765 IF GoldPieces > 1999 THEN PRINT "PLATE<2000>";
3770 PRINT
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3780 PRINT
3785 IF UserInput$ = "N" GOTO 3850
3790 IF UserInput$ = "L" THEN GoldPieces = GoldPieces - 1250: ArmorValue = 1: ArmorHitPoints = 7: GOTO 3850
3795 IF UserInput$ <> "C" OR GoldPieces >= 1500 GOTO 3810
3800 PRINT "** YOU HAVEN'T GOT THAT MUCH CASH ON HAND!"
3805 GOTO 3740
3810 IF UserInput$ = "C" THEN GoldPieces = GoldPieces - 1500: ArmorValue = 2: ArmorHitPoints = 14: GOTO 3850
3815 IF UserInput$ <> "P" OR GoldPieces >= 2000 GOTO 3830
3820 PRINT "** YOU CAN'T AFFORD PLATE ARMOR!"
3825 GOTO 3740
3830 IF UserInput$ = "P" THEN GoldPieces = GoldPieces - 2000: ArmorValue = 3: ArmorHitPoints = 21: GOTO 3850
3835 PRINT
3840 PRINT "** DON'T BE SILLY. CHOOSE A SELECTION."
3845 GOTO 3770
3850 IF GoldPieces < 1250 GOTO 3975
3855 PRINT
3860 PRINT "YOU HAVE"; GoldPieces; "GP'S LEFT WITH "; WeaponName$(WeaponValue + 1); " IN HAND."
3865 PRINT
3870 EquipmentType$ = "WEAPON"
PRINT
PRINT "THESE ARE THE TYPES OF "; EquipmentType$; " YOU CAN BUY :"
3880 PRINT "NOTHING<0> DAGGER<1250> ";
3885 IF GoldPieces > 1499 THEN PRINT "MACE<1500> ";
3890 IF GoldPieces > 1999 THEN PRINT "SWORD<2000>";
3895 PRINT
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3905 PRINT
3910 IF UserInput$ = "N" GOTO 3975
3915 IF UserInput$ = "D" THEN GoldPieces = GoldPieces - 1250: WeaponValue = 1: GOTO 3975
3920 IF UserInput$ <> "M" OR GoldPieces >= 1500 GOTO 3935
3925 PRINT "** SORRY SIR, I'M AFRAID I DON'T GIVE CREDIT!"
3930 GOTO 3865
3935 IF UserInput$ = "M" THEN GoldPieces = GoldPieces - 1500: WeaponValue = 2: GOTO 3975
3940 IF UserInput$ <> "S" OR GoldPieces >= 2000 GOTO 3960
3945 PRINT "** YOUR DUNGEON EXPRESS CARD - ";
3950 PRINT "YOU LEFT HOME WITHOUT IT!"
3955 GOTO 3865
3960 IF UserInput$ = "S" THEN GoldPieces = GoldPieces - 2000: WeaponValue = 3: GOTO 3975
3965 PRINT "** TRY CHOOSING A SELECTION!"
3970 GOTO 3895
3975 IF GoldPieces < 1000 GOTO 1950
3980 AttributeName$ = "STRENGTH"
PRINT
PRINT "DO YOU WANT TO BUY A POTION OF "; AttributeName$; " FOR 1000 GP'S";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
3990 IF UserInput$ <> "Y" GOTO 4020
3995 GoldPieces = GoldPieces - 1000
4000 PlayerStrength = CalculateCoordinate(PlayerStrength + RandomNumber(6))
4005 AttributeValue = PlayerStrength
PRINT
PRINT "YOUR "; AttributeName$; " IS NOW"; AttributeValue; "."
4015 GOTO 3975
4020 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 3980
4025 IF GoldPieces < 1000 GOTO 1950
4030 AttributeName$ = "INTELLIGENCE"
PRINT
PRINT "DO YOU WANT TO BUY A POTION OF "; AttributeName$; " FOR 1000 GP'S";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4040 IF UserInput$ <> "Y" GOTO 4070
4045 GoldPieces = GoldPieces - 1000
4050 PlayerIntelligence = CalculateCoordinate(PlayerIntelligence + RandomNumber(6))
4055 AttributeValue = PlayerIntelligence
PRINT
PRINT "YOUR "; AttributeName$; " IS NOW"; AttributeValue; "."
4065 GOTO 4025
4070 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 4030
4075 IF GoldPieces < 1000 GOTO 1950
4080 AttributeName$ = "DEXTERITY"
PRINT
PRINT "DO YOU WANT TO BUY A POTION OF "; AttributeName$; " FOR 1000 GP'S";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4090 IF UserInput$ <> "Y" GOTO 4120
4095 GoldPieces = GoldPieces - 1000
4100 PlayerDexterity = CalculateCoordinate(PlayerDexterity + RandomNumber(6))
4105 AttributeValue = PlayerDexterity
PRINT
PRINT "YOUR "; AttributeName$; " IS NOW"; AttributeValue; "."
4115 GOTO 4075
4120 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 4080
4125 IF (GoldPieces < 1000) OR (LampFlag = 1) GOTO 1950
4130 PRINT
4135 PRINT "DO YOU WANT TO BUY A LAMP FOR 1000 GP'S";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4145 IF UserInput$ <> "Y" GOTO 4175
4150 GoldPieces = GoldPieces - 1000
4155 LampFlag = 1
4160 PRINT
4165 PRINT "IT'S GUARANTEED TO OUTLIVE YOU!"
4170 GOTO 1950
4175 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 4130
4180 GOTO 1950
4185 MonsterStrength = 1 + INT(MonsterType / 2): MonsterHitPoints = MonsterType + 2: BribeAttempt = 1
4190 IF (CurseEffect(1, 4) > Treasure(1)) OR (BlindnessFlag = 1) OR (PlayerDexterity < RandomNumber(9) + RandomNumber(9)) GOTO 4700
4195 PRINT
4200 PRINT "YOU'RE FACING "; RoomContents$(MonsterType + 12); "!"
4205 PRINT
4210 PRINT "YOU MAY ATTACK OR RETREAT."
4215 IF BribeAttempt = 1 THEN PRINT "YOU CAN ALSO ATTEMPT A BRIBE."
4220 IF PlayerIntelligence > 14 THEN PRINT "YOU CAN ALSO CAST A SPELL."
4225 PRINT
4230 PRINT "YOUR STRENGTH IS"; PlayerStrength; "AND YOUR DEXTERITY IS"; PlayerDexterity; "."
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4240 IF UserInput$ <> "A" GOTO 4445
4245 IF WeaponValue <> 0 GOTO 4265
4250 PRINT
4255 PRINT "** POUNDING ON "; RoomContents$(MonsterType + 12); " WON'T HURT IT!"
4260 GOTO 4700
4265 IF BookFlag <> 1 GOTO 4285
4270 PRINT
4275 PRINT "** YOU CAN'T BEAT IT TO DEATH WITH A BOOK!"
4280 GOTO 4700
4285 IF PlayerDexterity >= RandomNumber(20) + (3 * BlindnessFlag) GOTO 4305
4290 PRINT
4295 PRINT "YOU MISSED, TOO BAD!"
4300 GOTO 4700
4305 MonsterName$ = RIGHT$(RoomContents$(MonsterType + 12), LEN(RoomContents$(MonsterType + 12)) - 2)
4310 IF LEFT$(MonsterName$, 1) = " " THEN MonsterName$ = MID$(MonsterName$, 2)
4315 PRINT
4320 PRINT "YOU HIT THE EVIL "; MonsterName$; "!"
4325 MonsterHitPoints = MonsterHitPoints - WeaponValue
4330 IF (MonsterType <> 9 AND MonsterType <> 12) GOTO 4355
4335 IF RandomNumber(8) <> 1 GOTO 4355
4340 PRINT
4345 PRINT "OH NO! YOUR "; WeaponName$(WeaponValue + 1); " BROKE!"
4350 WeaponValue = 0
4355 IF MonsterHitPoints > 0 GOTO 4700
4360 PRINT
4365 MonsterCount = MonsterCount - 1
4370 PRINT RoomContents$(MonsterType + 12); " LIES DEAD AT YOUR FEET!"
4375 IF HungerTurns > TurnCount - 60 GOTO 4395
4380 PRINT
4385 PRINT "YOU SPEND AN HOUR EATING "; RoomContents$(MonsterType + 12); EdiblePart$(RandomNumber(8)); "."
4390 HungerTurns = TurnCount
4395 IF PlayerX <> RunestaffLocation(1) OR PlayerY <> RunestaffLocation(2) OR CurrentLevel <> RunestaffLocation(3) THEN ON (1 - (MonsterType = 13)) GOTO 4420, 5305
4400 PRINT
4405 PRINT "GREAT ZOT! YOU'VE FOUND THE RUNESTAFF!"; CHR$(7)
4410 RunestaffLocation(1) = 0
4415 RunestaffFlag = 1
4420 GoldFound = RandomNumber(1000)
4425 PRINT
4430 PRINT "YOU NOW GET HIS HOARD OF"; GoldFound; "GP'S!"
4435 GoldPieces = GoldPieces + GoldFound
4440 GOTO 3105
4445 IF UserInput$ = "R" GOTO 4700
4450 IF UserInput$ <> "C" GOTO 4595
4455 IF PlayerIntelligence >= 15 OR BribeAttempt <= 1 GOTO 4475
4460 PRINT
4465 PRINT "** YOU CAN'T CAST A SPELL NOW!"
4470 GOTO 4195
4475 PRINT
4480 PRINT "WHICH SPELL (WEB, FIREBALL, DEATHSPELL)";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4490 PRINT
4495 IF UserInput$ <> "W" GOTO 4515
4500 PlayerStrength = PlayerStrength - 1
4505 WebCount = RandomNumber(8) + 1
4510 ON (1 - (PlayerStrength < 1)) GOTO 4700, 4910
4515 IF UserInput$ <> "F" GOTO 4560
4520 DamagePoints = RandomNumber(7) + RandomNumber(7)
4525 PlayerStrength = PlayerStrength - 1
4530 PlayerIntelligence = PlayerIntelligence - 1
4535 IF (PlayerIntelligence < 1) OR (PlayerStrength < 1) GOTO 4910
4540 PRINT "IT DOES"; DamagePoints; "POINTS WORTH OF DAMAGE."
4545 PRINT
4550 MonsterHitPoints = MonsterHitPoints - DamagePoints
4555 GOTO 4355
4560 IF UserInput$ = "D" GOTO 4580
4565 PRINT
4570 PRINT "** TRY ONE OF THE OPTIONS GIVEN."
4575 GOTO 4195
4580 PRINT "DEATH . . . ";
4585 IF PlayerIntelligence < RandomNumber(4) + 15 THEN PRINT "YOURS!": PlayerIntelligence = 0: GOTO 4910
4590 PRINT "HIS!": MonsterHitPoints = 0: GOTO 4360
4595 IF UserInput$ = "B" AND BribeAttempt <= 1 GOTO 4615
4600 PRINT
4605 PRINT "** CHOOSE ONE OF THE OPTIONS LISTED."
4610 GOTO 4195
4615 IF TreasureCount <> 0 GOTO 4635
4620 PRINT
4625 PRINT "ALL I WANT IS YOUR LIFE!"
4630 GOTO 4700
4635 TreasureIndex = RandomNumber(8)
4640 IF Treasure(TreasureIndex) = 0 GOTO 4635
4645 PRINT
4650 PRINT "I WANT "; RoomContents$(TreasureIndex + 25); ". WILL YOU GIVE IT TO ME";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4660 IF UserInput$ = "N" GOTO 4700
4665 IF UserInput$ <> "Y" THEN PRINT YesNoPrompt$: GOTO 4645
4670 Treasure(TreasureIndex) = 0
4675 TreasureCount = TreasureCount - 1
4680 PRINT
4685 PRINT "OK, JUST DON'T TELL ANYONE ELSE."
4690 VendorFlag = VendorFlag + (LocationMap(CalculateDimension(CurrentLevel)) = 25)
4695 GOTO 1950
4700 BribeAttempt = 2
4705 IF WebCount <= 0 GOTO 4720
4710 WebCount = WebCount - 1
4715 IF WebCount = 0 THEN PRINT : PRINT "THE WEB JUST BROKE!"
4720 MonsterName$ = RIGHT$(RoomContents$(MonsterType + 12), LEN(RoomContents$(MonsterType + 12)) - 2)
4725 IF LEFT$(MonsterName$, 1) = " " THEN MonsterName$ = MID$(MonsterName$, 2)
4730 IF WebCount <= 0 GOTO 4750
4735 PRINT
4740 PRINT "THE "; MonsterName$; " IS STUCK AND CAN'T ATTACK NOW!"
4745 GOTO 4805
4750 PRINT
4755 PRINT "THE "; MonsterName$; " ATTACKS!"
4760 IF PlayerDexterity < RandomNumber(7) + RandomNumber(7) + RandomNumber(7) + 3 * BlindnessFlag GOTO 4780
4765 PRINT
4770 PRINT "WHAT LUCK, HE MISSED YOU!"
4775 GOTO 4805
4780 PRINT
4785 PRINT "OUCH! HE HIT YOU!"
4790 DamagePoints = MonsterStrength
IF ArmorValue > 0 THEN
    DamagePoints = DamagePoints - ArmorValue
    ArmorHitPoints = ArmorHitPoints - ArmorValue
    IF DamagePoints < 0 THEN
        ArmorHitPoints = ArmorHitPoints - DamagePoints
        DamagePoints = 0
    END IF
    IF ArmorHitPoints < 0 THEN
        ArmorHitPoints = 0
        ArmorValue = 0
        PRINT
        PRINT "YOUR ARMOR HAS BEEN DESTROYED . . . GOOD LUCK!"
    END IF
END IF
PlayerStrength = PlayerStrength - DamagePoints
4800 IF PlayerStrength < 1 GOTO 4910
4805 IF UserInput$ <> "R" GOTO 4195
4810 PRINT
4815 PRINT "YOU HAVE ESCAPED!"
4820 PRINT
4825 PRINT "DO YOU WANT TO GO NORTH, SOUTH, EAST, OR WEST";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
4835 IF UserInput$ = "N" OR UserInput$ = "S" OR UserInput$ = "E" OR UserInput$ = "W" GOTO 2440
4840 PRINT
4845 PRINT "** DON'T PRESS YOUR LUCK, "; Race$(PlayerRace); "!"
4910 PRINT CHR$(7)
4915 PrintStars
4920 PRINT "A NOBLE EFFORT, OH FORMERLY LIVING "; Race$(PlayerRace); "!"
4925 PRINT
4930 PRINT "YOU DIED DUE TO LACK OF ";
4935 IF PlayerStrength < 1 THEN PRINT "STRENGTH."
4940 IF PlayerIntelligence < 1 THEN PRINT "INTELLIGENCE."
4945 IF PlayerDexterity < 1 THEN PRINT "DEXTERITY."
4950 PRINT
4955 GameEndReason = 1
4960 PRINT "AT THE TIME YOU DIED, YOU HAD :"
4965 GOTO 5055
4970 GameEndReason = 0
4975 PRINT
4980 PRINT "YOU LEFT THE CASTLE WITH";
4985 IF OrbFlag = 0 THEN PRINT "OUT";
4990 PRINT " THE ORB OF ZOT."
4995 PRINT
5000 IF OrbFlag = 0 GOTO 5030
5005 PRINT
5010 PRINT "AN INCREDIBLY GLORIOUS VICTORY!!"
5015 PRINT
5020 PRINT "IN ADDITION, YOU GOT OUT WITH THE FOLLOWING :"
5025 GOTO 5050
5030 PRINT
5035 PRINT "A LESS THAN AWE-INSPIRING DEFEAT."
5040 PRINT
5045 PRINT "WHEN YOU LEFT THE CASTLE, YOU HAD :"
5050 IF GameEndReason = 0 THEN PRINT "YOUR MISERABLE LIFE!"
5055 FOR TreasureIndex = 1 TO 8
5060 IF Treasure(TreasureIndex) = 1 THEN PRINT RoomContents$(TreasureIndex + 25)
5065 NEXT TreasureIndex
5070 PRINT WeaponName$(WeaponValue + 1); " AND "; WeaponName$(ArmorValue + 5);
5075 IF LampFlag = 1 THEN PRINT " AND A LAMP";
5080 PRINT
5085 PRINT "YOU ALSO HAD"; FlareCount; "FLARES AND"; GoldPieces; "GOLD PIECES"
5090 IF RunestaffFlag = 1 THEN PRINT "AND THE RUNESTAFF"
5095 PRINT
5100 PRINT "AND IT TOOK YOU"; TurnCount; "TURNS!"
5105 PRINT
5110 PRINT "ARE YOU FOOLISH ENOUGH TO WANT TO PLAY AGAIN";
PRINT
PRINT "YOUR CHOICE";
INPUT UserInput$
UserInput$ = LEFT$(UserInput$, 1)
5120 PRINT
5125 IF UserInput$ <> "Y" GOTO 5155
5130 PRINT "SOME "; Race$(PlayerRace); "S NEVER LEARN!"
5135 PRINT
5140 PRINT "PLEASE BE PATIENT WHILE THE CASTLE IS RESTOCKED."
5145 PRINT
5150 GOTO 1105
5155 IF UserInput$ <> "N" THEN PRINT YesNoPrompt$: GOTO 5110
5160 PRINT "MAYBE DUMB "; Race$(PlayerRace); " IS NOT SO DUMB AFTER ALL!"
5165 PRINT
5170 GOTO 5580
5175 PRINT
5180 PRINT "GREAT UNMITIGATED ZOT!"
5185 PRINT
5190 PRINT "YOU JUST FOUND ***THE ORB OF ZOT***!"
5195 PRINT
5200 PRINT "THE RUNESTAFF HAS DISAPPEARED!"
5205 RunestaffFlag = 0
5210 OrbFlag = 1
5215 OrbLocation(1) = 0
5220 GOTO 3105
5225 DATA AN EMPTY ROOM,.,THE ENTRANCE,E,STAIRS GOING UP,U
5230 DATA STAIRS GOING DOWN,D,A POOL,P,A CHEST,C,GOLD PIECES,G
5235 DATA FLARES,F,A WARP,W,A SINKHOLE,S,A CRYSTAL ORB,O
5240 DATA A BOOK,B,A KOBOLD,M,AN ORC,M,A WOLF,M,A GOBLIN,M,AN OGRE,M
5245 DATA A TROLL,M,A BEAR,M,A MINOTAUR,M,A GARGOYLE,M,A CHIMERA,M
5250 DATA A BALROG,M,A DRAGON,M,A VENDOR,V,THE RUBY RED,T
5255 DATA THE NORN STONE,T,THE PALE PEARL,T,THE OPAL EYE,T
5260 DATA THE GREEN GEM,T,THE BLUE FLAME,T,THE PALANTIR,T,THE SILMARIL,T
5265 DATA X,"?",NO WEAPON," SANDWICH"
5270 DATA DAGGER," STEW",MACE," SOUP",SWORD," BURGER",NO ARMOR," ROAST"
5275 DATA LEATHER," FILET",CHAINMAIL," TACO",PLATE," PIE"
5280 DATA HOBBIT,ELF,MAN,DWARF
5285 PlayerX = RandomNumber(8): PlayerY = RandomNumber(8)
5290 IF LocationMap(CalculateDimension(CurrentLevel)) <> 101 GOTO 5285
5295 LocationMap(CalculateDimension(CurrentLevel)) = RoomType
5300 RETURN
5305 PRINT
5310 PRINT "YOU GET ALL HIS WARES :"
5315 PRINT "PLATE ARMOR"
5320 ArmorValue = 3: ArmorHitPoints = 21
5325 PRINT "A SWORD"
5330 WeaponValue = 3
5335 PRINT "A STRENGTH POTION"
5340 PlayerStrength = CalculateCoordinate(PlayerStrength + RandomNumber(6))
5345 PRINT "AN INTELLIGENCE POTION"
5350 PlayerIntelligence = CalculateCoordinate(PlayerIntelligence + RandomNumber(6))
5355 PRINT "A DEXTERITY POTION"
5360 PlayerDexterity = CalculateCoordinate(PlayerDexterity + RandomNumber(6))
5365 IF LampFlag = 0 THEN PRINT "A LAMP": LampFlag = 1
5370 GOTO 4420
5580 END

FUNCTION AdjustLargeNumber (InputNumber)
    AdjustLargeNumber = InputNumber + 100 * (InputNumber > 99)
END FUNCTION

FUNCTION AdjustNumber (InputNumber)
    AdjustNumber = InputNumber + 8 * ((InputNumber = 9) - (InputNumber = 0))
END FUNCTION

FUNCTION CalculateCoordinate (InputNumber)
    CalculateCoordinate = -InputNumber * (InputNumber < 19) - 18 * (InputNumber > 18)
END FUNCTION

FUNCTION CalculateDimension (Level)
    SHARED PlayerX, PlayerY
    CalculateDimension = 64 * (Level - 1) + 8 * (PlayerX - 1) + PlayerY
END FUNCTION

SUB PrintStars
    FOR Index = 1 TO 64
    PRINT "*";
    NEXT Index
    PRINT
    PRINT
END SUB

FUNCTION RandomNumber (MaxValue)
    RandomNumber = 1 + INT(RND(1) * MaxValue)
END FUNCTION

SUB ChooseRace (PlayerRace, PlayerStrength, PlayerDexterity, OtherPoints)
    DIM Race$(4)
    Race$(1) = "HOBBIT"
    Race$(2) = "ELF"
    Race$(3) = "MAN"
    Race$(4) = "DWARF"
    
    DO
        PRINT "ALL RIGHT, BOLD ONE."
        PRINT "YOU MAY BE AN ELF, DWARF, MAN, OR HOBBIT."
        PRINT
        PRINT "YOUR CHOICE";
        INPUT UserInput$
        UserInput$ = UCASE$(LEFT$(UserInput$, 1))
        
        PlayerRace = 0
        SELECT CASE UserInput$
            CASE "H": PlayerRace = 1  ' Hobbit
            CASE "E": PlayerRace = 2  ' Elf
            CASE "M": PlayerRace = 3  ' Man
            CASE "D": PlayerRace = 4  ' Dwarf
        END SELECT
        
        IF PlayerRace > 0 THEN
            PlayerStrength = PlayerStrength + 2 * PlayerRace
            PlayerDexterity = PlayerDexterity - 2 * PlayerRace
            OtherPoints = OtherPoints + 4 * (PlayerRace = 2)  ' Extra points for Elf
            PRINT "You have chosen to be a "; Race$(PlayerRace)
            Race$(3) = "HUMAN"
            EXIT DO
        ELSE
            PRINT
            PRINT "** THAT WAS INCORRECT. PLEASE TYPE E, D, M, OR H."
        END IF
    LOOP
END SUB

SUB ChooseSex (PlayerSex, PlayerRace)
    DIM Race$(4)
    Race$(1) = "HOBBIT"
    Race$(2) = "ELF"
    Race$(3) = "MAN"
    Race$(4) = "DWARF"

    DO
        PRINT "WHICH SEX DO YOU PREFER";
        PRINT
        PRINT "YOUR CHOICE";
        INPUT UserInput$
        UserInput$ = UCASE$(LEFT$(UserInput$, 1))

        SELECT CASE UserInput$
            CASE "M"
                PlayerSex = 1
                EXIT DO
            CASE "F"
                PlayerSex = 0
                EXIT DO
            CASE ELSE
                PRINT "** CUTE "; Race$(PlayerRace); ", REAL CUTE. TRY M OR F."
        END SELECT
    LOOP

    PRINT
END SUB

SUB ChooseArmor (ArmorValue, ArmorHitPoints, GoldPieces, PlayerRace)
    DIM Race$(4)
    Race$(1) = "HOBBIT"
    Race$(2) = "ELF"
    Race$(3) = "MAN"
    Race$(4) = "DWARF"

    EquipmentType$ = "ARMOR"
    ArmorValue = 0

    DO
        PRINT
        PRINT "THESE ARE THE TYPES OF "; EquipmentType$; " YOU CAN BUY :"
        PRINT "PLATE<30> CHAINMAIL<20> LEATHER<10> NOTHING<0>"
        PRINT
        PRINT "YOUR CHOICE";
        INPUT UserInput$
        UserInput$ = UCASE$(LEFT$(UserInput$, 1))

        SELECT CASE UserInput$
            CASE "P": ArmorValue = 3
            CASE "C": ArmorValue = 2
            CASE "L": ArmorValue = 1
            CASE "N": ArmorValue = 0
            CASE ELSE
                ArmorValue = -1  ' Invalid input
        END SELECT

        IF ArmorValue >= 0 THEN
            EXIT DO
        ELSE
            PRINT
            PRINT "** ARE YOU A "; Race$(PlayerRace); INVALID VALUE"?"
        END IF
    LOOP

    ArmorHitPoints = ArmorValue * 7
    GoldPieces = GoldPieces - ArmorValue * 10
END SUB

SUB BuyFlares (FlareCount, GoldPieces)
    IF GoldPieces >= 1 THEN
        DO
            PRINT "FLARES COST 1 GP EACH. HOW MANY DO YOU WANT";
            INPUT UserInput$
            FlareCount = VAL(UserInput$)
            PRINT

            IF FlareCount > 0 OR ASC(UserInput$) = 48 THEN
                IF FlareCount <= GoldPieces THEN
                    EXIT DO
                ELSE
                    PRINT "** YOU CAN ONLY AFFORD"; GoldPieces; "."
                    PRINT
                END IF
            ELSE
                PRINT "** IF YOU DON'T WANT ANY, JUST TYPE 0 (ZERO)."
                PRINT
            END IF
        LOOP
    END IF 
    GoldPieces = GoldPieces - FlareCount
END SUB
