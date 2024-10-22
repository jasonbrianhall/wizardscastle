#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <QString>

namespace AsciiArt {

#ifdef _WIN32
// Simple bubble art versions for Windows
const QString ENTRANCE_AA = 
    " .--------.\n"
    "|ENTRANCE |\n"
    " '--------'\n";

const QString STAIRSUP_AA = 
    " .-----.\n"
    "|STAIRS|\n"
    "|  UP  |\n"
    " '-----'\n";

const QString STAIRSDOWN_AA = 
    " .-----.\n"
    "|STAIRS|\n"
    "| DOWN |\n"
    " '-----'\n";

const QString POOL_AA = 
    " .----.\n"
    "| POOL|\n"
    "|~~~~~|\n"
    " '----'\n";

const QString CHEST_AA = 
    " .-----.\n"
    "|CHEST |\n"
    "|[===] |\n"
    " '-----'\n";

const QString CRYSTALORB_AA = 
    " .----.\n"
    "| ORB |\n"
    "| (o) |\n"
    " '----'\n";

const QString BOOK_AA = 
    " .----.\n"
    "|BOOK |\n"
    "|[:=] |\n"
    " '----'\n";

const QString KOBOLD_AA = 
    " .------.\n"
    "|KOBOLD |\n"
    "| >:-<  |\n"
    " '------'\n";

const QString ORC_AA = 
    " .---.\n"
    "|ORC |\n"
    "| >:[ |\n"
    " '---'\n";

const QString WOLF_AA = 
    " .----.\n"
    "|WOLF |\n"
    "| ^w^ |\n"
    " '----'\n";

const QString GOBLIN_AA = 
    " .------.\n"
    "|GOBLIN |\n"
    "| >:-}  |\n"
    " '------'\n";

const QString OGRE_AA = 
    " .----.\n"
    "|OGRE |\n"
    "| >:O |\n"
    " '----'\n";

const QString TROLL_AA = 
    " .-----.\n"
    "|TROLL |\n"
    "| >:-D |\n"
    " '-----'\n";

const QString BEAR_AA = 
    " .----.\n"
    "|BEAR |\n"
    "| ʕ·ᴥ·ʔ|\n"
    " '----'\n";

const QString MINOTAUR_AA = 
    " .-------.\n"
    "|MINOTAUR|\n"
    "| Ѷ(:-)Ѷ |\n"
    " '-------'\n";

const QString GARGOYLE_AA = 
    " .-------.\n"
    "|GARGOYLE|\n"
    "| }:->   |\n"
    " '-------'\n";

const QString CHIMERA_AA = 
    " .------.\n"
    "|CHIMERA|\n"
    "| ⟟:≎:⟟ |\n"
    " '------'\n";

const QString BALROG_AA = 
    " .------.\n"
    "|BALROG |\n"
    "| }:⚕:{ |\n"
    " '------'\n";

const QString DRAGON_AA = 
    " .------.\n"
    "|DRAGON |\n"
    "| ⟟:▼:⟟ |\n"
    " '------'\n";

const QString VENDOR_AA = 
    " .------.\n"
    "|VENDOR |\n"
    "| [$_$] |\n"
    " '------'\n";

#else

const QString ENTRANCE_AA = 
"    _____________________\n"
"   /        EXIT         \\\n"
"  /     ___________       \\\n"
" /     |  _______  |       \\\n"
"|      | |       | |        |\n"
"|      | |       | |        |\n"
"|      | |       | |        |\n"
"|      | |     _ | |        |\n"
"|      | |    |_|| |        |\n"
"|      | |     __| |        |\n"
"|      | |    |  | |        |\n"
"|      | |    |  | |        |\n"
"|      | |()  |  | |        |\n"
"|      | |()  |  | |        |\n"
"|      | |()  |  | |        |\n"
"|      |_|()__|__| |        |\n"
"|     /           \\ |       |\n"
"|    /             \\|       |\n"
"|   /               \\       |\n"
"|  /                 \\      |\n"
"| /                   \\     |\n"
"|/                     \\    |\n";

const QString STAIRSUP_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠰⡇⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⣠⠽⠶⣞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⢠⠚⣡⠀⠀⢎⠓⠤⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⡞⢸⣿⠀⠀⢸⠉⠓⠲⠾⠃⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠓⠚⣿⠀⠀⣸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⡏⢀⠦⣄⠉⢲⠀⠀⠀⠀⣶⠒⠒⠒⠒⢶\n"
"⠀⠀⠀⠀⠀⠀⠀⣸⠁⣸⠠⢸⠁⢸⢠⠶⠶⠴⣿⠀⠀⠀⠀⢸\n"
"⠀⠀⠀⠀⠀⠀⢡⠇⢠⠋⠀⣬⣷⣾⣼⡇⠈⠀⠁⠁⠀⠀⣠⠾\n"
"⠀⠀⠀⠀⠀⠀⠈⢓⣋⣀⣀⡇⠀⠀⠀⠀⠀⠀⢀⣠⠔⠋⠀⠀\n"
"⠀⠀⠀⢀⡀⠀⠀⣸⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠋⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⢸⡍⠉⠉⠉⠀⠀⠀⠀⢀⡠⠖⠉⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⡏⠉⠉⠉⠁⠀⠀⠀⠀⢀⡤⠚⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⡇⠀⠀⠀⠀⠀⣀⡴⠚⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠓⠒⠒⠒⠒⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";

const QString STAIRSDOWN_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣶⣿⣿⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⡿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣏⠀⣿⣿⣿⣿⣿⣿⣷⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡗⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⠷⠀⣿⣿⣿⣿⣿⣿⠀⠈⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠀⠀⣝⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣯⣻⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣷⣝⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⠀⠀⠀⠀⢀⣠⣴⣿⣿⣿⣿⡿⠙⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⢠⣶⣿⣿⣿⣿⡿⠛⠁⠀⠀⠸⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠹⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⢻⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⠀⠀⠀⠀⠈⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⣿⣿⠀⠀⠀⠀⠀⠸⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣀⣀⣀⣀⣀⣀⣙⣟⣋⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⠿⠿⢿⢿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉\n";

const QString POOL_AA = 
".*.         \n"
"      .*.*.*.*.      \n"
"    .*.* ___ *.*.    \n"
"  .*.*  (   )  *.*.  \n"
".*.*    (   )    *.*.\n"
"*:*  ___\\   /___  *:*\n"
"*:* (           ) *:*\n"
"*:* )  ~~~ ~~~  ( *:*\n"
"*:* ( ~~~~~~~~~ ) *:*\n"
"*:* )  ~~~ ~~~  ( *:*\n"
"*:* (  ~~~~~~~  ) *:*\n"
"*:* ) ~~~~~~~~~ ( *:*\n"
"*:* (___________) *:*\n"
" *:*    \\___/    *:* \n"
"  *:*           *:*  \n"
"   *:*.*.*.*.*.*:*   \n"
"     '*.*.*.*.*'\n";

const QString CHEST_AA = 
"____...------------...____\n"
"       _.-\"` /o/__ ____ __ __  __ \\o\\_`\"-._\n"
"     .'     / /                    \\ \\     '.\n"
"     |=====/o/======================\\o\\=====|\n"
"     |____/_/________..____..________\\_\\____|\n"
"     /   _/ \\_     <_o#\\__/#o_>     _/ \\_   \\\n"
"     \\_____________\\##########/_____________/\n"
"      |===\\!/========================\\!/===|\n"
"      |   |=|          .---.         |=|   |\n"
"      |===|o|=========/     \\========|o|===|\n"
"      |   | |         \\() ()/        | |   |\n"
"      |===|o|======{'-.) A (.-'}=====|o|===|\n"
"      | __/ \\__     '-.\\uuu/.-'    __/ \\__ |\n"
"      |============== .'.'^'.'.============|\n"
"      |  _\\o/   __  {.' __  '.} _   _\\o/  _|\n"
"      `\"\"\"\"-\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-\"\"\"\"`\n";

const QString CRYSTALORB_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣤⡤⡤⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡴⣺⡛⣛⡿⠟⠛⢹⡯⣩⠍⢙⡯⢟⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⠟⣗⣉⣿⣗⣿⡗⢄⠀⠚⠓⠦⠤⠛⣮⢾⣅⢹⡋⠙⠷⣄⡀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⢞⣑⣲⣛⣿⣿⣿⣼⠟⠛⠒⠱⠀⠀⠀⠀⠀⠉⢿⣋⣷⡜⡆⠀⠈⢻⣢⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⡟⠛⠛⠀⠀⠀⠘⢣⡀⠀⠘⣵⢽⢆⠀⠀⠑⠷⣁⢷⣬⣓⣆⠀⠙⡷⡄⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⢿⣿⡿⣿⠀⠀⠀⠀⠀⠀⠀⠈⠁⢀⡀⢁⣮⣋⡻⡢⠀⠰⣊⡉⠘⢯⡮⣠⡄⠈⣿⡄⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⢠⣿⡟⢯⣯⣉⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠖⠷⠎⢩⡍⠁⠀⠀⠘⣍⠧⢾⡾⡬⣻⣤⠸⣿⡄⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⣾⡿⠁⠀⠹⣟⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣢⡀⠀⠀⠀⢀⡝⠏⣇⠸⣿⣨⣨⣆⣿⣷⠀\n"
"⠀⠀⠀⠀⠀⠀⢀⣿⢷⡀⠀⡀⠩⠹⡯⢷⢤⣀⣤⢀⣀⢤⣄⣀⠠⣴⡟⠛⢣⡀⠀⢀⣼⣥⣿⡿⠦⠾⠚⠀⢸⡁⣻⡄\n"
"⠀⠀⠀⠀⠀⠀⢸⣿⡯⢝⣶⠁⠀⠀⠈⠛⢯⢿⠿⢿⣒⣦⣤⣦⣦⣦⠛⠻⣇⣸⣓⡿⣾⣯⣦⢧⣦⠠⠄⣀⢼⣵⢼⡇\n"
"⠀⠀⠀⠀⠀⠀⠸⣿⢇⢿⣍⣇⠀⠀⠀⣠⣤⠈⠀⠀⠈⠙⠫⠽⢟⣱⢻⣦⡛⣯⣿⠧⠞⠱⠷⠀⠀⠀⠸⠝⢾⣟⣿⡇\n"
"⠀⠀⠀⠀⠀⠀⠀⣿⢷⣤⣛⣶⣆⠀⠈⠉⠉⠀⠀⣤⠀⠀⢀⡀⠀⠀⣉⡺⠚⠂⠋⠐⣶⣤⣀⣤⡀⣠⡀⠐⠻⠾⣾⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⢸⣟⢓⢻⡟⠽⣵⠄⠀⠀⠀⠀⠀⠀⢀⣼⢿⣷⠀⠉⠙⢤⣄⠀⠸⠾⣾⣿⣿⡿⣿⠃⠀⠀⣰⡏⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⢻⡏⠋⠫⣷⢻⡧⣦⠀⠀⠀⢀⠀⠙⢯⣯⡈⠶⠗⠀⠳⣷⣶⣖⣵⣿⣿⠭⠛⠃⣀⡀⢠⡟⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣆⠀⠀⠀⢠⣬⠁⣤⣀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠈⠻⠟⠛⠋⠀⠀⠠⣺⠿⣣⡟⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⢆⠀⠈⠚⣆⠼⢷⠿⣥⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⣠⣤⡝⢀⡴⠋⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠀⠀⠀⢀⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣶⣷⣶⣶⣤⡀⠀⡀⣤⣄⡀⠀⢀⣠⣴⣶⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⡆⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣽⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⡻⣿⣿⠁⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⣿⣿⣿⡿⠿⠛⠛⠛⠿⠿⠿⠟⠛⠛⠛⠛⢿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⡄⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣷\n";

const QString BOOK_AA = 
".-~~~~~~~~~-._       _.-~~~~~~~~~-.\n"
"            __.'              ~.   .~              `.__\n"
"          .'//                  \\./                  \\\\`.\n"
"        .'//                     |                     \\\\`.\n"
"      .'// .-~\"\"\"\"\"\"\"~~~~-._     |     _,-~~~~\"\"\"\"\"\"\"~-. \\\\`.\n"
"    .'//.-\"                 `-.  |  .-'                 \"-.\\\\`.\n"
"  .'//______.============-..   \\ | /   ..-============.______\\\\`.\n"
".'______________________________\\|/______________________________`.\n";

const QString KOBOLD_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢶⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⡲⢶⣶⣶⣶⣿⣯⣷⣏⡿⡴⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢿⣿⣜⢧⢯⡹⣝⠾⣹⢏⡿⣻⠿⣿⣿⣿⣿⣶⣝⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣨⣿⣿⣮⡳⡝⡾⣱⢫⡜⣧⢻⡱⣏⢿⣿⣿⣿⣿⣷⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⣹⣿⣿⣿⣏⣳⣭⢳⡝⣎⣷⣹⣮⣷⣽⣻⣿⣷⣿⣻⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣿⣿⣿⣿⣿⣿⣳⣟⣾⣽⣻⡾⠿⢛⣻⣟⢿⣿⣻⣿⣿⣿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠻⣿⣿⣟⡿⣯⣟⡳⢞⡱⢮⠏⠀⠀⣸⢿⣿⣦⠙⣭⢿⣿⣟⣿⣻⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣽⣿⢻⡼⢭⣏⡽⡏⠀⠀⠸⣧⣼⣿⣾⡆⢩⡞⣭⢻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⢿⣏⢮⣳⢚⢧⣞⣱⡇⠀⠀⠀⢿⢾⣿⣯⠇⢸⡝⣮⢳⠮⣝⡻⢿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣿⢻⡝⣮⢳⢎⡷⢎⡻⢯⡝⢯⣻⣄⠀⠀⠈⠛⠮⢙⢀⡾⣹⢎⣗⡻⡼⣹⣟⣽⠿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣾⡵⣏⣾⣜⣫⡝⢧⣿⡻⣵⢻⢷⣥⣤⣤⣤⡴⣯⢳⡝⣺⢬⡳⣝⡱⢯⢿⣟⢮⡟⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣿⣷⣿⣶⣯⣷⣺⣽⢞⣧⠿⣬⣯⣷⣹⣜⡣⢏⣞⢱⣯⡜⣧⢻⡹⣞⡻⢮⡝⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠛⢹⣷⣯⣟⡷⣿⣯⠿⠿⢷⡾⠾⠷⠾⠷⠾⠾⢧⣮⣿⣿⣱⡿⣶⣹⢣⢿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⢯⡿⣳⣿⣩⣛⣽⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠤⠤⣀⠀⠛⣿⠽⣚⣓⣉⣉⣉⣉⣩⠩⢔⣒⡠⠀⠀⠀⠀⠀⠀⠀⢰⣶⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡰⠁⢠⣤⠟⢹⡽⣶⣉⡓⠒⡒⠆⠰⠄⠀⠀⠉⠉⠓⡀⠀⠀⠀⢀⣴⣀⣀⡹⣿⣿⣿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⡔⠊⠀⠘⠁⢎⣇⣾⣷⣖⣦⡴⣤⠴⣒⢀⣤⣄⣠⠄⢠⡄⢣⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⠤⢦⡽⢒⡥⡿⠁⠞⠛⠮⣅⠒⡬⠨⡺⣷⠸⣇⣾⡧⡛⠀⠀⠀⢻⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠒⠛⡞⣁⡢⡖⢀⣤⣾⢡⠂⠀⠀⠀⠀⠀⢷⠿⢽⡇⠀⠀⠀⣷⢻⡟⡿⣟⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⡟⣿⣿⡯⣿⡷⠀⠀⠀⠀⠀⠀⠀⠀⠞⣄⠀⠀⡐⣯⣻⡽⣽⣻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢼⣿⡱⢿⡐⠁⠐⡚⠤⡀⠀⠀⠀⠀⡸⣷⢶⡱⢌⠏⢻⢗⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣅⣿⣿⣹⢻⡑⠤⡄⢀⡀⢆⣀⠠⠐⣔⣾⣿⢯⡷⣦⣽⣬⠅⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⢠⣤⣤⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢾⣟⠛⠪⠽⢊⣲⠥⣇⡌⠀⢀⣠⡮⢙⡊⡽⠯⠟⠛⠉⣸⠃⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠘⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣤⣤⣤⣤⣄⣠⡪⠳⠓⣈⠥⡐⢸⣯⣿⣿⣿⣿⣯⠿⡑⠲⣜⢵⠀⠀⠀⠀⠎⡘⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠛⠛⠛⠛⠻⢿⡿⠾⢿⡷⢂⠬⡒⢁⠔⣵⣿⣶⣷⣦⣤⣤⣤⣤⣤⣥⣼⣤⣤⣤⣤⣤⣥⣀⣠⣤⣤⡠⣔⠶⠊⠑⡺⠁⠆⡤⡀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣦⣤⣅⡀⠘⠞⣄⠈⡌⠨⣿⡟⠉⠉⠉⠉⠉⠉⠙⡻⢻⠻⢟⠛⠛⠛⠛⠓⢿⣿⣿⣿⠗⡠⠡⣠⠞⠁⠀⠀⠀⣁⡋⢖⣤\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣶⣾⣿⣿⣿⣽⣯⣿⢀⡄⠀⣨⠉⠉⠁⢡⠀⢀⠎⠀⠀⠀⢈⣤⣧⠁⣹⡄⠀⠀⠀⠀⠀⠀⠉⠉⠈⠱⠇⣴⣆⡄⣶⢩⠿⠶⠈⠁⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣰⣿⣿⣿⣿⣿⣿⣽⣯⣿⣿⣿⣿⠟⢣⠞⠁⢀⢠⢠⣿⢀⠎⠀⠀⢰⠆⢨⡏⣵⣿⡟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠉⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⣀⣤⣤⣼⣿⠿⣟⢯⡳⣝⢮⣳⢹⣫⣿⣻⣞⡏⣂⣠⣴⣶⣾⣶⣾⣿⣼⣶⣄⠀⡀⡤⢊⢠⣽⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⢤⣤⡔⣶⢻⢿⡹⣏⡟⡼⢎⡟⣼⢣⡝⣮⡷⣞⡿⣽⣞⡷⣯⣟⡿⣿⣻⣞⣷⣻⣟⡾⣽⢯⣷⣧⣿⡿⣷⣶⣟⣞⣻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠉⠛⠼⢯⣾⣵⣳⠾⣽⣽⣺⢷⣻⣟⣷⣻⡽⢻⠷⣏⣟⠭⢗⣺⣿⣳⣟⣾⠳⣏⠾⣥⣛⠶⣻⣯⣟⣷⣻⢾⡼⣹⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠉⠉⠛⠛⠋⠭⠯⠧⠷⠶⠥⠭⠯⠛⠒⠛⠉⠁⣼⣯⢷⡾⣭⢻⡜⣏⠶⣭⣛⡽⣷⣻⡾⣽⣻⢾⣵⣳⡳⣶⣤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣟⣯⣿⣽⣻⡽⣞⡿⣶⣣⣽⣶⣷⣿⣿⣿⣿⣾⣓⠛⠛⠛⠛⠛⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠓⠛⠿⠿⠿⠿⠿⠿⠿⠛⠛⠃⠀⠀⠀⠀⠀⠀\n";

const QString ORC_AA = 
"⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⡂⠢⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡒⣄⣼⣿⣟⠿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠱⡀⠡⡀⢂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠼⠛⠛⣯⣿⣿⡲⢉⡣⣧⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠘⣁⠒⡄⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡔⠈⢡⠊⠉⠣⣠⠤⣵⣾⣿⣿⣍⢗⣻⣜⢅⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠘⠖⠨⣆⠐⡀⠀⠀⠀⠀⠀⠀⠀⢀⢼⣧⣈⡤⠂⠥⢀⠗⠢⢁⠛⢿⣿⣿⣞⣶⣴⣾⣿⡦⠄⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠈⢇⠱⡣⠀⠄⠀⠀⠀⢀⠄⠐⠊⡜⡟⠿⣿⣷⣶⠟⣀⡚⢀⠂⠌⢿⣿⣿⣿⣿⣿⣿⢇⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢣⠙⣕⡀⢂⠀⢠⣼⣶⣧⡴⣱⣧⣃⣨⣿⣿⣯⡁⠐⡀⠊⢄⣼⣿⣿⣿⣿⣻⠙⣑⠄⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⢹⡈⣎⣄⣴⣿⣿⣿⣿⣶⣿⠉⠁⢹⣿⣿⣿⣿⣷⣶⣿⣿⣿⣷⡟⣫⣼⣿⣧⣧⣵⣷⡞⠀⠀⠀\n"
"⠀⠀⠀⠀⢠⣄⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⢫⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠸⣿⣿⣿⣿⣿⣟⠃⠀⠀\n"
"⠀⠀⠀⠀⠀⠻⣿⠛⠁⠻⠟⠿⡿⢿⠿⠁⠀⠀⠀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⠀⠀⠜⠛⢿⣿⣿⣿⡄⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⡁⡔⡀⢤⣧⠀⠀⢀⡾⣿⣿⣿⣿⣿⣿⣿⣿⣻⣣⣿⣿⣷⣦⣀⠀⠀⣻⡏⠠⠬⠢⡀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢿⣾⣿⡇⠀⢠⣾⢿⣳⣛⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣟⣿⣶⣄⠹⣇⡵⣷⣶⡆\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣷⡤⡟⣼⣣⢯⡝⣾⣽⢿⣿⣿⣿⣿⣿⣿⣿⣳⡭⣿⣣⣟⣵⠉⠁⠴⠟⠁\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣩⣶⣿⣿⣿⣧⡻⠂⠀⢸⣿⠟⢹⣿⠁⠀⣩⣿⣿⣷⣿⡿⠏⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣽⣿⣿⣿⣿⠏⠀⠀⠀⠈⠁⠀⠈⠇⠀⠐⣿⣿⣿⣿⡛⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⡿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣧⣀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣶⣆⡀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠛⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀\n";

const QString WOLF_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣷⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣿⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⣿⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣶⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⢿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠏⢹⣿⡿⠁⠈⠛⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⠟⠁⠀⢻⣿⣿⠉⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠇⠀⠀⠀⠈⢿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠋⠀⠀⠀⠀⢻⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⢀⡾⠀⠀⠀⢿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⠃⠀⠀⠸⣄⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⠀⣼⡇⠀⠀⠀⠈⣿⣿⣷⡄⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣶⣿⢿⠏⠀⠀⠀⠀⣿⡄⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣧⣿⡇⠀⠀⠀⠀⠙⠈⠻⣿⣾⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⡿⠋⠘⠀⠀⠀⠀⢀⣿⣷⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⠟⠉⣿⡄⠀⠀⠀⠀⠀⠀⠙⢿⡿⣿⣶⣤⣤⣀⣀⣀⣀⣤⣴⣿⡿⣿⠏⠀⠀⠀⠀⠀⠀⠀⣼⡏⠙⢿⣿⣿⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠠⠟⢹⣿⣿⠃⠀⠀⠈⠿⣆⡀⠀⠀⠀⠀⠀⠀⠙⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠀⠐⠁⠀⠀⠀⠀⠀⠀⢀⡾⠋⠀⠀⠈⢻⣿⡏⠙⠳⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠇⠀⠀⠀⠀⠀⠀⠙⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠁⠀⠀⠀⠀⠀⠈⢿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠖⠀⠀⠀⣀⣤⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢦⣄⡀⠀⠀⠀⢾⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣧⡖⠀⣠⣾⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣦⡀⠳⣼⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣤⣾⣿⠟⠀⠀⠀⠀⠀⠀⢀⣠⠀⠀⠀⠀⠀⠀⠀⢠⣿⣷⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣦⣼⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⣠⣶⣿⣭⣤⣤⣴⣾⣆⣤⠀⠀⣾⣿⣿⡄⠀⢠⠀⣿⣷⣤⣬⣬⣽⣷⣤⡀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⡏⠀⠀⠀⢀⣾⣿⠟⠁⠈⠉⠛⠋⠹⣿⠚⣷⣼⣿⣿⣿⣿⣤⡿⣸⡟⠉⠛⠋⠉⠈⠙⢿⣿⣆⠀⠀⠀⠘⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⣟⣿⡿⠿⣿⠿⢷⡶⣄⠀⢻⡆⣿⣿⣿⣿⣿⣿⣿⡇⣿⠁⢀⣴⣶⠿⣿⠿⠿⣿⣾⡆⠀⠀⠀⠸⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢀⣼⣿⡿⠛⢱⣿⠃⠀⠀⠀⠀⢸⣿⣤⠀⠻⢷⣌⣉⣸⣧⣀⣧⣿⣿⣿⣿⣿⣿⣿⣧⣇⣠⣿⣈⣹⣶⠿⠃⣤⣽⣿⠻⠀⠀⠀⠀⢻⣧⠙⢿⣿⣿⣆⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⢠⣾⣿⠋⠀⣰⣿⡇⠀⢀⣤⣾⠃⠈⠉⣽⣷⣤⠀⠈⠉⠉⣻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣉⠉⠉⠁⠤⣶⣿⡉⠉⠀⢻⣦⣀⠀⠈⣿⣧⡀⠘⢿⣿⣆⠀⠀⠀⠀\n"
"⠀⠀⠀⢠⣿⡿⠁⢀⣼⣿⣿⣠⣶⣿⣿⡇⠀⠐⠾⠿⠿⢶⣶⣾⠿⣶⣶⣿⡿⢿⣿⣿⣿⣿⣿⣿⠟⣿⣶⣤⠾⢿⣶⣶⠾⠿⠿⠖⠀⠈⣿⣿⣷⣦⣸⣿⣷⡄⠀⠹⣿⣧⠀⠀⠀\n"
"⠀⠀⢀⣾⡟⠁⢀⣾⣿⣿⠿⠋⢠⣿⣿⢀⡀⠀⠀⠀⠒⠋⠉⠁⠀⠘⣿⠟⠀⠀⢿⣿⣿⣿⣿⠃⠀⠙⣿⣿⠀⠀⠉⠙⠓⠂⠀⠀⠀⡀⢸⣿⣇⠉⠻⢿⣿⣿⣆⠀⠹⣿⡆⠀⠀\n"
"⠀⠀⢸⡟⠁⢀⣾⡿⠋⠁⠀⠀⢸⣿⣿⡟⠁⠀⢀⣀⠀⠀⠀⠀⠀⢸⡿⠀⠀⠀⠈⢿⣿⣿⠁⠀⠀⠀⠙⣿⠀⠀⠀⠀⠀⠀⣀⠀⠀⠹⣾⣿⣿⠀⠀⠀⠉⠻⢿⡆⠀⠹⣷⡀⠀\n"
"⠀⠀⡿⠁⠀⡸⠋⠀⠀⠀⠀⠀⢸⣿⣿⢃⣤⣶⣿⠁⠀⠀⠀⠀⠀⢸⠃⠀⠀⠀⠀⠀⢿⠃⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠹⣿⣦⡀⢹⣿⣿⠃⠀⠀⠀⠀⠈⢻⡄⠀⢹⡇⠀\n"
"⠀⢸⡇⠀⢀⣡⣤⣶⣶⣶⡇⠀⢸⣿⣿⡿⢿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠇⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⣿⣿⢿⣿⣿⡿⠀⠀⢷⣶⣶⣦⣄⡁⠀⠈⣇⠀\n"
"⠀⣸⠁⣰⣾⠟⠹⣿⣿⣿⠁⠀⠈⣿⡿⠁⠘⣿⣇⠀⠀⠀⠀⡀⠀⠀⠀⠀⣤⣶⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⡀⠀⠀⠀⢀⣿⡟⠀⢹⣿⡇⠀⠀⢸⣿⣿⡟⠙⠿⣶⡀⢸⡀\n"
"⢀⣿⣾⠏⠀⠀⠀⢻⣿⣿⡄⠀⠀⢸⡇⠀⠀⠘⣿⣄⠀⠀⣸⡃⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⣿⠀⠀⢀⣾⡟⠀⠀⠀⡿⠁⠀⠀⣸⣿⣿⠇⠀⠀⠈⠳⣾⡇\n"
"⣸⠟⠁⠀⠀⠀⠀⠘⣿⣿⡇⠀⠀⠀⠁⠀⠀⠀⠈⢿⣦⡀⢹⣧⠀⠀⠀⠘⠛⠛⠛⣿⣿⣿⡟⠛⠛⠟⠀⠀⠀⢀⣿⠀⣠⣿⠏⠀⠀⠀⠀⠁⠀⠀⠀⣿⣿⡏⠀⠀⠀⠀⠀⠙⢷\n"
"⠋⠀⠀⠀⠀⠀⠀⠀⠸⣿⣷⠀⠀⠀⣀⡀⠀⠀⠀⠀⠙⢿⣦⣿⣄⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⢀⣾⣯⣾⠟⠁⠀⠀⠀⠀⢀⡀⠀⠀⢸⣿⡟⠁⠀⠀⠀⠀⠀⠀⠈\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣇⠀⢰⣿⠻⢦⣄⠀⠀⠀⠀⠙⣿⣿⣦⣀⠀⠀⠀⢈⣿⣿⣿⡋⠀⠀⠀⢀⣠⣾⣿⡟⠁⠀⠀⠀⢀⣴⡾⢻⡇⠀⢠⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣇⢸⣿⠀⠀⠙⢷⣄⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⠛⠛⠉⠙⠛⢻⣿⣿⣿⣿⣿⠋⠀⠀⠀⢀⡴⠛⠁⠀⢸⡇⢠⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⠀⠀⠀⠀⠘⢷⡀⠀⠀⠘⣿⣿⣿⣿⣿⣷⣦⣤⣤⣶⣿⣿⣿⣿⣿⡏⠀⠀⢀⣴⠋⠀⠀⠀⠀⢸⣷⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⡇⠀⠀⠀⠀⠀⠻⣦⠀⠀⠹⡟⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠙⡿⠀⠀⢠⡾⠁⠀⠀⠀⠀⢀⣾⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠹⣧⡀⠀⠁⡄⢻⣿⣿⣿⣿⣿⣿⣿⣿⠁⣄⠁⠀⣰⡿⠀⠀⠀⠀⠀⠀⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣷⡀⣼⣧⠀⢿⠟⣿⣿⣿⡿⣿⠃⢀⣿⠀⣰⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⠀⠈⣇⢻⣿⣿⠀⡟⠀⢸⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⠟⣿⡄⠀⠹⠘⣿⡿⠘⠀⠀⣿⡟⢿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⢿⣷⠀⠀⠀⣿⡇⠀⠀⢠⣿⠁⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡟⠀⠈⣿⡄⠀⠀⢿⠀⠀⠀⣾⡏⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠃⠀⠀⠹⣷⠀⠀⠀⠀⠀⣰⡿⠀⠀⠀⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣧⢀⡄⣄⢀⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⠀⣿⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣆⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⡟⠀\n";

const QString GOBLIN_AA = 
",      ,\n"
"            /(.-\\\"\\\"-.)\\\n"
"        |\\  \\/      \\/  /|\n"
"        | \\ / =.  .= \\ / |\n"
"        \\( \\   o\\/o   / )/\n"
"         \\_, '-/  \\-' ,_/\n"
"           /   \\__/   \\\n"
"           \\ \\__/\\__/ /\n"
"         ___\\ \\|--|/ /___\n"
"       /`    \\      /    `\\\n"
"      /       '----'       \\\n";

// Shrek???
const QString OGRE_AA = 
"⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⢀⣠⣤⣤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⢀⣴⠟⠉⠀⠀⠀⠈⠻⣦⡀⠀⠀⠀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣀⢀⣾⠿⠻⢶⣄⠀⠀⣠⣶⡿⠶⣄⣠⣾⣿⠗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⢻⣿⣿⡿⣿⠿⣿⡿⢼⣿⣿⡿⣿⣎⡟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡟⠉⠛⢛⣛⡉⠀⠀⠙⠛⠻⠛⠑⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣧⣤⣴⠿⠿⣷⣤⡤⠴⠖⠳⣄⣀⣹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣀⣟⠻⢦⣀⡀⠀⠀⠀⠀⣀⡈⠻⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⡿⠉⡇⠀⠀⠛⠛⠛⠋⠉⠉⠀⠀⠀⠹⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⡟⠀⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠃⠀⠈⠑⠪⠷⠤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣦⣼⠛⢦⣤⣄⡀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠑⠢⡀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⢀⣠⠴⠲⠖⠛⠻⣿⡿⠛⠉⠉⠻⠷⣦⣽⠿⠿⠒⠚⠋⠉⠁⡞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢦⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢀⣾⠛⠁⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠤⠒⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢣⠀⠀⠀\n"
"⠀⠀⠀⠀⣰⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣑⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡇⠀⠀\n"
"⠀⠀⠀⣰⣿⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣧⣄⠀⠀⠀⠀⠀⠀⢳⡀⠀\n"
"⠀⠀⠀⣿⡾⢿⣀⢀⣀⣦⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡰⣫⣿⡿⠟⠻⠶⠀⠀⠀⠀⠀⢳⠀\n"
"⠀⠀⢀⣿⣧⡾⣿⣿⣿⣿⣿⡷⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⢀⡴⢿⣿⣧⠀⡀⠀⢀⣀⣀⢒⣤⣶⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇\n"
"⠀⠀⡾⠁⠙⣿⡈⠉⠙⣿⣿⣷⣬⡛⢿⣶⣶⣴⣶⣶⣶⣤⣤⠤⠾⣿⣿⣿⡿⠿⣿⠿⢿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇\n"
"⠀⣸⠃⠀⠀⢸⠃⠀⠀⢸⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⠟⡉⠀⠀⠀⠈⠙⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇\n"
"⠀⣿⠀⠀⢀⡏⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⠿⠿⠛⠛⠉⠁⠀⠀⠀⠀⠀⠉⠠⠿⠟⠻⠟⠋⠉⢿⣿⣦⡀⢰⡀⠀⠀⠀⠀⠀⠀⠁\n"
"⢀⣿⡆⢀⡾⠀⠀⠀⠀⣾⠏⢿⣿⣿⣿⣯⣙⢷⡄⠀⠀⠀⠀⠀⢸⡄⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣿⣻⢿⣷⣀⣷⣄⠀⠀⠀⠀⢸⠀\n"
"⢸⠃⠠⣼⠃⠀⠀⣠⣾⡟⠀⠈⢿⣿⡿⠿⣿⣿⡿⠿⠿⠿⠷⣄⠈⠿⠛⠻⠶⢶⣄⣀⣀⡠⠈⢛⡿⠃⠈⢿⣿⣿⡿⠀⠀⠀⠀⠀⡀\n"
"⠟⠀⠀⢻⣶⣶⣾⣿⡟⠁⠀⠀⢸⣿⢅⠀⠈⣿⡇⠀⠀⠀⠀⠀⣷⠂⠀⠀⠀⠀⠐⠋⠉⠉⠀⢸⠁⠀⠀⠀⢻⣿⠛⠀⠀⠀⠀⢀⠇\n"
"⠀⠀⠀⠀⠹⣿⣿⠋⠀⠀⠀⠀⢸⣧⠀⠰⡀⢸⣷⣤⣤⡄⠀⠀⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡆⠀⠀⠀⠀⡾⠀⠀⠀⠀⠀⠀⢼⡇\n"
"⠀⠀⠀⠀⠀⠙⢻⠄⠀⠀⠀⠀⣿⠉⠀⠀⠈⠓⢯⡉⠉⠉⢱⣶⠏⠙⠛⠚⠁⠀⠀⠀⠀⠀⣼⠇⠀⠀⠀⢀⡇⠀⠀⠀⠀⠀⠀⠀⡇\n"
"⠀⠀⠀⠀⠀⠀⠻⠄⠀⠀⠀⢀⣿⠀⢠⡄⠀⠀⠀⣁⠁⡀⠀⢠⠀⠀⠀⠀⠀⠀⠀⠀⢀⣐⡟⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⢠⡇\n";

const QString TROLL_AA = 
"⠀⠀\n"
"        .-\\\"\\\"\\\"\\\".\n"
"       /       \\\n"
"   __ /   .-.  .\\\n"
"  /  `\\  /   \\/  \\\n"
"  |  _ \\/   .==.==.\n"
"  | (   \\  /____\\__\\\n"
"   \\ \\      (_()(_()\n"
"    \\ \\            '---._\n"
"     \\                   \\_\n"
"  /\\ |`       (__)________/\n"
" /  \\|     /\\___/\n"
"|    \\     \\||VV\n"
"|     \\     \\|\\\"\\\"\\\"\\\",\n"
"|      \\     ______)\n"
"\\       \\  /`\n"
"         \\(\n";

const QString BEAR_AA = 
"⠀⠀\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀                                ⠀⠀ ⠒⠈⠀⠄⣠⣤⣤⣀⠄⠐⣂⣐\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                  ⠀⠀⠀⠀⠀⠀⠀⠀⣐⠀⠀⠀⠀⢸⣿⣿⣿⣿⣷⣾⣿⣝⣷⣔\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠤⠀⠄⠠⠄⠤⠀⠄⠠⠄⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣶⡿⣿⣿⣿⢛⢛⣿⣿⡿⠛⡋⣵⣾⣿⣿⣽⣽⣿⣦\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢀⣠⡴⠿⣟⣛⣋⣷⡟⠛⠋⢰⣾⣿⠏⠀⠀⠠⠈⢉⣒⣏⣤⣽⣿⣿⣷⣐\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢄⣀⣤⠾⣛⣱⣾⣿⠿⠛⠯⢀⣠⡦⠉⢿⣿⡋⢀⡅⠄⠀⢀⣂⣻⣿⣿⣟⣛⣩⣶⡞⣻⢿⣶⣶⡄\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⠶⣞⣿⠽⡒⠛⠉⢛⠛⣛⡀⢀⣲⣿⡿⣥⢠⣿⣿⢦⣿⣷⠠⣀⢻⣻⣿⣯⡿⢏⡉⠢⠄⣀⣁⡈⢻⡟⠃\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠯⣥⣾⣿⠋⠜⠂⠉⠀⠀⠈⣰⣶⡆⣿⣿⣗⣟⠸⣿⣿⣾⡻⠿⣿⣬⡊⠛⠿⠿⢃⣿⣷⣿⣷⣦⣤⣶⡞⡁⠁\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣽⣿⣿⡇⠎⠀⠀⠀⠀⠀⠀⣿⣿⡇⣿⣿⣿⣜⢎⡻⢿⣿⣿⣦⢨⡙⠃⠁⠘⠀⣾⣹⠃⣿⢏⣙⡙⡋\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠄⠀⠀⣼⢏⣾⣿⣿⡍⡎⠀⠄⠰⠒⠂⠄⠀⣿⣯⠱⢬⠻⣿⣿⣦⡙⠦⠈⠻⠿⠇⢀⠀⡀⢀⣆⣹⠛⢻⡿⠿⠿⣧⢡\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠠⠂⠀⠠⠀⠀⣰⠏⣿⡿⠟⠻⠥⠂⣐⣒⣒⣂⠀⠠⢀⣿⣿⣧⠀⢁⣾⣿⣿⣻⣴⣦⣶⣷⣿⣿⣿⣿⣿⡏⠍⠙⠫⠍⠻⠶⠏⠐\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢂⠀⢀⡖⠀⢀⣴⢿⣟⣫⣶⡟⢻⠿⣿⣏⠍⠍⠿⣛⠷⣾⣿⣿⠃⡀⠀⠿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠉⠙⠁⠀⠀⠀⠀⠀⠀⠀⠁\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠠⠁⢀⡇⠀⠠⣿⣳⣿⣿⣻⣿⣿⣶⣤⣆⠰⠀⠐⠄⠀⠱⢦⣍⣻⢷⣌⠂⠄⠀⢰⣿⣿⣿⣿⣿⣿⣿⠐\n"
" ⠀⠀⠀⠀⠀⠀⠄⠀⠀⢰⠠⠆⠀⡠⠈⠁⠈⣸⣿⠘⣿⣿⣷⣽⣿⣻⡛⠋⠡⠀⠀⠀⠀⡈⠐⠈⠝⢛⡛⠛⠛⠛⠿⣿⣿⣿⣿⠿⠻⣿⡇⡆\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂⠀⠀⠸⠁⠀⣰⡿⣿⣿⣷⣝⢻⣿⣿⣿⣿⣿⣿⣶⣿⣶⠿⠖⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣾⡛⣿⣿⣀⡀⢘⣷\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠀⠀⢀⡴⠟⣹⣿⣾⣿⣿⣷⣿⣼⣯⣹⣤⡐⣤⣁⣁⣭⣭⣁⣂⠂⣤⠠⡀⠘⢿⣿⣿⣭⣷⡾⢍⡉⢻⡮⣿⡐⡀\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⢢⡴⢋⠔⡃⢿⣿⢇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠻⠿⢷⣄⠠⣤⣾⣿⠟⠛⢦⡈⢿⣾⣷⣬⠳⣄⡀⠄\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⣴⣫⡶⢠⠀⢃⠻⣿⣿⡏⢽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠙⠀⣙⣷⣮⣥⣤⡿⣷⡞⣿⣿⠿⢿⣿⣷⣌⡛⢦⣅⠂\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⢋⠀⠀⠀⠀⠡⠹⣿⣧⡈⠹⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣶⣽⣴⣿⢿⢀⣾⣿⣿⣿⣷⣟⣋⣐⡃⠢⢛⢻⣿⣦⡍⠛⢦⣄⢀\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣷⠂⠂⠀⠀⠀⠀⠐⠌⢿⣿⠘⢷⣿⢻⣿⣿⣿⣿⣿⣿⣿⡮⣙⠛⠿⢿⠿⠛⢹⣿⠿⢿⣿⣿⣿⣿⣿⣭⣭⣀⠀⠐⠉⢻⣿⣌⠢⢹⡇⠄\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⣳⡦⠀⠀⠀⠀⠀⠀⡈⠘⣿⣌⠊⢻⣦⠌⡙⠛⠿⣿⣿⣿⠁⠐⠨⠂⠀⠀⠁⣘⣿⠙⠣⢉⠿⢿⣿⣿⣿⣿⣿⣧⣥⡄⠤⠹⣿⣶⡀⣿⣜\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⡟⠸⠀⠀⠀⠀⠀⠀⢿⣿⣿⣮⣷⣮⣻⣷⣭⣭⣤⣿⢍⠡⠄⠀⠀⢀⣿⣆⠁⣸⣿⡅⠀⠀⠀⠈⠙⡛⢿⣿⣿⣿⣿⣷⣶⣤⢈⠻⠇⠹⠛⢷⣄\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣷⣧⠀⠄⠀⠀⠀⠀⠐⠩⠛⢻⡗⢄⢿⣿⣿⣿⡿⢋⠀⠀⠀⠀⢀⣿⣿⠋⡂⢟⣿⠐⠂⠀⠀⠀⠀⠀⠀⠉⠙⠿⣿⣿⣿⣿⣯⣄⡁⢀⠀⢈⢻⡇\n"
" ⠀⠀⠀⠀⠀⠀⡀⠀⠀⣿⡽⣿⣷⣄⢀⠀⠀⠀⠀⠀⠈⠈⢷⣦⢈⣷⣿⠫⣤⠁⠀⠁⢀⣴⣾⡿⠏⠀⠀⢸⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠿⣿⣿⣿⣶⣤⣌⠘⣇⢡\n"
" ⠀⠀⠀⠀⠀⢀⠀⡀⠀⢹⡇⡘⢿⣿⣄⠱⠀⠀⠠⠐⢄⡀⠂⡛⣿⣿⣿⣧⣌⡝⢧⢀⣾⡿⠋⠅⠓⡐⢂⣼⡏⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⢀⣿⠸\n"
" ⠀⠀⠀⠀⠀⠠⠀⠀⢠⢸⡇⢆⠢⡙⠿⣿⣧⠡⠀⢦⡀⠒⠀⠀⠈⢿⣿⣿⣿⣿⣦⣄⠑⠀⣰⣸⣿⣷⡿⡋⠛⣶⣄⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠂⠡⠘⣯⣿⣿⣯⡾⠇\n"
" ⠀⠀⠀⠀⠀⠰⠀⠄⡀⣼⣷⡦⡆⠀⢠⣶⢩⠀⠀⠸⣿⠆⠈⣀⠀⢘⣿⣿⣿⣿⣿⣿⣿⣼⣿⣿⡿⠁⠀⠀⠠⡻⠿⣦⡀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠉\n"
" ⠀⠀⠀⠀⠀⠐⢀⠠⠡⢨⣾⣶⡼⡄⣿⣿⣦⢡⠀⠐⠃⠀⠐⠀⠀⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠄⠀⠀⠀⠀⠀⢁⠺⣿⣆\n"
" ⠀⠀⠀⠀⠀⠀⢸⠀⢸⢈⣽⣧⡀⣇⢻⣿⣿⠈⠀⡀⠀⠀⠀⣈⣸⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣗⣀⣀⣀⡀⠀⠀⠀⠐⢹⣿⣷\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⠈⠌⢿⣿⢣⡍⣨⡟⠡⣠⣶⡀⠠⢐⣠⣿⣿⣼⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⠛⠀⠀⠀⠀⢈⢻⣟\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⡐⣥⣿⡅⣠⣾⠏⠜⡘⣿⣿⣾⣿⡟⠟⣥⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣳⣶⣦⠀⠠⣻⣿⡇\n"
" ⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⡏⣲⣿⡏⢈⠀⠁⢁⡁⠀⡌⣰⣷⣷⠏⠿⣿⣿⣿⣿⣿⣯⣭⣭⣭⡽⠶⠛⠋⠍⠝⠻⠿⠟⠀⣠⡿⠟⠡\n"
" ⠀⠀⡀⠀⠀⠀⠀⣤⣿⢣⢰⣿⣿⣾⠦⡑⢄⠤⢐⣩⣶⣟⠫⡔⠪⠓⠈⢬⡍⢱⣟⡗⣲⣴⣶⣧⣤⡥⠆⠤⠤⢀⣂⣤⡾⠋⠈\n"
" ⠀⠀⠀⠀⠀⢠⡾⡛⠥⡹⠌⠙⠋⣵⣾⣿⣿⢾⣻⣽⢾⣛⡷⣦⠡⠄⠀⠀⠡⣿⢷⣒⠀⠑⡌⣴⠷⣟⣛⠛⠛⢏⢻⡿⣿⣳⡆⠣\n"
" ⠀⠀⠀⠀⠀⣿⣿⠇⡃⠀⠀⠀⠐⠀⡈⢿⣾⠟⢧⡉⢿⣏⣿⣿⢷⢡⠀⠀⠀⠿⣶⣬⣭⣭⣭⣭⣭⣤⣅⣀⠀⠀⠤⠄⠄⣻⣿⣷⠠\n"
" ⠀⠀⠀⠀⠀⠙⢿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣬⣍⣸⣿⣿⣹⣿⣿⠿⠂⠠⠀⠀⠀⠐⠉⠉⠉⠉⠁⠀⠐⠊⠭⠛⠛⠛⡛⠛⠛⠿⠋\n"
" ⠀⠀⠀⠀⠀⠀⠀⠐⠀⠀⠒⠒⠒⠒⠒⠚⠉⠭⠩⣿⠏⠻⠇⢙⠐⠈⠤⢀⠀⢀⠂⠀⠀⠀⠀⠐⠒⠒⠂⠀⠀⠀⠀⡀⢀\n";

const QString MINOTAUR_AA = 
".      .\n"
"    |\\____/|\n"
"   (\\|----|/)\n"
"    \\ 0  0 /\n"
"     |    |\n"
"  ___/\\../\\____\n"
" /     --       \\\n"
"/  \\         /   \\\n"
"|    \\___/___/(   |\n"
"\\   /|  }{   | \\  )\n"
"\\  ||__}{__|  |  |\n"
" \\  |;;;;;;;\\  \\ / \\_______\n"
"  \\ /;;;;;;;;| [,,[|======'\n"
"    |;;;;;;/ |     /\n"
"    ||;;|\\   |\n"
"     ||;;/|   /\n"
"    \\_|:||__|\n"
"     \\ ;||  /\n"
"     |= || =|\n"
"     |= /\\ =|\n"
"     /_/  \\_\\\n";

const QString GARGOYLE_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢤⠀⠀⢀⡄⠀⠀⠀⠀⢀⡀⠀⠀⠤⠤⠄⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠆⠇⢦⣀⡴⢠⠀⠀⣤⣰⣈⣀⣀⣀⡀⠀⠰⠶⠶⠆⠀⠀⣤⠈⠰⠆⡄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠁⢀⠠⠂⢉⣁⡮⢀⡀⠈⠭⡉⠭⠭⠉⠁⢀⡒⠂⠠⠄⣀⠀⠀⠀⠀⠀⢀⠽⢐⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠡⠂⢀⢀⠌⠉⠀⠁⠈⢢⡉⠂⠄⡁⠢⡀⠀⠀⠀⠈⠀⠒⠀⠬⢐⠠⢠⡊⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠆⠂⠒⡒⠆⠉⢀⠠⢐⠅⠁⠀⠀⠀⠀⠀⠱⡐⡀⠀⠀⠐⠌⡀⢄⠀⠀⠀⠀⠀⠀⠀⠀⠁⠊⠕⡢⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠸⢂⠩⠢⠀⠀⠀⠀⠀⡠⠚⠔⠂⠈⠀⠐⢑⡒⣢⡬⠅⠠⡄⡀⠀⠀⠀⠀⠀⠑⡰⠀⠀⠀⠀⠈⠠⡑⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠈⠢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠑⢕⠱⢌⠀⠄⠔⣨⢦⡆⠀⠀⠀⣘⠄⡇⠀⠀⢘⢄⠁⢪⡀⢷⢄⠀⠀⠀⠀⣧⠀⠀⠀⠀⠀⠀⠈⢀⠡⠀⠀⠀⠀⠀⠀⠀⠀⡀⠠⠀⠁⠰⡓⠄⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠁⢂⠝⡒⣛⡎⡡⠉⠀⠐⠰⡇⠀⠀⠀⢠⠅⡘⠀⠜⠉⠢⠑⡑⠄⠀⠀⢣⠀⠀⠀⠀⠀⠀⠀⠀⠁⡑⡀⠀⠀⠀⢀⠔⠁⠀⠀⠀⠀⠀⠈⠞⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⢀⡦⣾⡋⠸⢹⠢⠛⢠⡀⠒⣰⡄⡀⢠⠃⡈⠀⠀⠀⠀⠀⢡⡑⡌⢢⡜⡈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠔⠄⠀⡰⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⣿⣾⣸⣷⣿⡍⢯⢁⣠⡍⣇⡶⠀⠀⠱⣬⢠⠱⣶⢀⠶⠶⣩⢀⢡⡌⡼⠁⠁⠀⠀⠀⠈⠁⠰⠆⡄⡀⠈⡌⣤⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠠⠝⠳⠟⠓⣋⠄⠊⠈⠁⡀⡈⠇⢀⣠⣐⣘⠐⠤⠅⠪⢐⠄⠁⢌⠈⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠢⢠⡹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⢤⡣⠁⠃⠊⠀⠀⠀⠀⢀⠊⠰⢀⠴⠀⠠⡀⣀⠀⠁⠂⢄⡈⢉⡀⠠⠈⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠃⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⠔⢁⡞⢸⠀⠀⡇⠋⠢⡀⠀⠀⠈⠫⡀⡀⠀⠀⠸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠋⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⢀⠂⢀⠀⠢⢀⠃⠐⡁⠀⠁⢀⠀⠀⠈⠫⠃⠀⠀⢃⠀⠀⢲⡀⠀⡀⡀⠀⠀⠀⠀⠐⠀⠀⠀⠀⠀⢈⡣⠀⠑⠒⠒⢠⡤⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡘⠈⢀⡞⠁⢀⡃⢂⠀⠈⠦⣀⠀⡇⠀⠀⠀⠁⠀⠀⠸⡀⠀⢸⠀⡈⠀⠀⣁⢁⢨⠀⠀⠀⠀⠀⣀⢀⢫⣣⢄⡠⠐⠸⠁⠀⠠⣀⣀⡠⣄⡲⠄\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀⠅⠖⠀⠠⠂⠈⠢⡑⢄⠀⠀⢈⢇⠀⠀⠀⠀⠀⠀⢰⠇⢀⠁⠁⠀⢀⠠⠂⠉⠉⠁⠠⡀⠀⠀⢁⠕⠩⠐⠉⢱⢠⠁⠀⠀⠀⠀⠈⠉⠉⠀⠀\n"
"⠀⠀⣀⣀⡀⣀⠀⠀⣀⡂⡌⠀⠀⣰⣅⡀⠀⢀⠰⡀⣢⢀⠠⢫⠆⠀⠀⠀⡠⠾⠁⣀⠀⠈⠁⠉⠀⢀⠔⠉⠀⠁⠢⢀⠉⢈⡡⠀⠀⠀⠀⠘⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⢿⣾⣿⣧⣤⡆⠀⠀⠰⡆⢀⣼⣧⢰⣶⣶⣀⢠⠎⠀⠈⠀⠷⠈⡄⠀⠈⠀⠀⠀⠀⠀⠁⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠈⠛⣢⣭⠻⣓⠠⠤⠤⠠⠜⠻⠗⡮⢩⢉⠻⠀⠀⠀⣀⠀⣀⣀⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";

const QString CHIMERA_AA = 
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣖⠗⠺⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠛⠛⠛⢯⡧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠐⣷⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⢠⡠⠶⠶⠤⠀⠀⠀⠀⠀⠀⠀⢸⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⣠⡞⡫⠷⣟⣿⠖⢤⠀⢀⠀⢠⡴⠾⣷⡬⠯⢿⠗⠒⠒⠒⠀⠀⠀⠀⠀⠀⠘⣧⡈⠐⠠⠤⢀⡀⠀⠀⠀⠀⠀\n"
"⠀⣠⣊⠞⠿⠞⣸⣿⠙⡹⣧⣿⣼⠇⠙⢗⣶⣧⡀⣀⡒⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠯⠿⢶⣦⣀⡑⢄⠀⠀⠀\n"
"⠀⢿⣦⣄⣤⣠⣾⡿⣪⣻⣷⣭⡞⡵⢀⡠⠛⢸⣿⣋⡐⢼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⢣⠀⠀\n"
"⠀⠀⢙⣭⣯⣶⣷⣷⣹⣿⣿⣛⣭⡕⣫⣾⡆⣾⣿⡏⠀⠈⢲⣤⢠⣤⠐⠀⢠⣤⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡗⢸⠀⠀\n"
"⠀⠀⠈⠛⢻⣿⣿⣿⣿⣟⣷⡝⢿⣿⣿⣿⣿⣿⣿⣷⡴⢾⣿⠿⢟⡇⠀⠀⠀⠘⢸⣿⣶⣄⡀⠀⠀⠀⣠⡾⢣⠎⠀⠀\n"
"⠀⠀⠀⠀⠀⡘⣿⣿⣍⣻⣧⣷⠘⣷⣷⢽⢇⣟⣿⠃⣔⠾⠛⠀⠘⡅⣄⠀⠀⠀⠀⠀⢽⡿⣷⣶⣴⢶⣭⠶⠊⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠉⣹⣷⣝⣽⢠⣾⣵⣿⡶⠏⢉⢿⡱⡚⠉⠀⠀⠀⡀⢾⣎⣷⠂⠀⠀⠀⢞⢷⡄⠈⠉⠁⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠹⣿⣹⣧⣸⢿⣿⣏⠀⢀⢨⢨⡛⢎⠲⡄⡄⡼⣗⣿⡗⣯⡄⠐⢦⡄⠈⠓⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠘⠋⣿⣷⣿⣿⣿⠊⠀⡸⠃⣾⣻⣄⡿⣻⣯⡽⢿⣿⣿⣿⣼⣦⣤⡀⠀⠀⠙⠋⢱⣦⣆⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠈⠫⣿⣿⣿⡿⡀⠈⠁⠈⣿⣿⣿⢿⡿⠟⢰⣿⣿⣵⢿⣛⣿⠿⣟⢿⣦⣤⠀⢀⠈⢠⣷⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⠄⠐⡏⢺⡏⠉⠁⠀⠀⠀⠘⣿⣻⢟⣿⣿⣟⠀⠀⠉⠑⠚⠻⣷⢙⠉⢻⡄⠀⠀⠀\n"
"⠀⠀⣀⣀⠄⣀⣀⣴⣾⣿⣥⡽⢻⡄⠈⠉⡟⠀⠀⢀⣀⣀⣀⡀⠈⣻⣿⣿⣿⣿⣄⣤⣤⠤⠠⠀⢹⣧⠐⡤⢇⠙⠿⠧\n"
"⠀⢠⣤⣤⣶⣷⣾⡿⣿⡾⢛⣁⣸⠃⢀⣛⣣⣤⡿⠟⠛⠚⣓⣩⣼⣏⢿⣣⠟⠋⠛⠉⠀⠁⠀⢀⣰⠷⠀⣀⣾⠃⠀⠀\n"
"⢀⣿⣿⣿⣿⣽⣹⣾⡿⣷⣾⣿⠟⢂⣾⣮⡥⣶⣶⣴⣶⣿⣿⣿⣿⣿⡿⣥⣶⢶⣶⣶⣶⣶⣶⡿⢿⡟⢩⣿⣭⣶⣿⡃\n"
"⠘⠋⠉⠀⠉⠊⠉⠄⠛⠛⠻⠟⠛⠋⠋⠉⠋⠉⠙⠋⠙⠛⠛⠉⠁⠐⠤⡈⠃⡤⠝⡤⠿⠟⠻⠿⠿⠷⠾⠉⠋⠓⠂⠁\n";

const QString BALROG_AA = 
")  (\n"
"             (   ) )\n"
"              ) ( (\n"
"     /\\  .-\"\"\"-.  /\\\n"
"    //\\\\/  ,,,  \\//\\\\\n"
"    |/\\| ,;;;;;, |/\\|\n"
"    //\\\\\\;-\"\"\"-;///\\\\\n"
"   //  \\/   .   \\/  \\\\\n"
"  (| ,-_| \\ | / |_-, |)\n"
"    //`__\\.-.-./__`\\\\\n"
"   // /.-(() ())-.\\ \\\\\n"
"  (\\ |)   '---'   (| /)\n"
"   ` (|           |) `\n"
"     \\)           (/\n"
"      |    )(    |\n"
"     /|\\  /--\\  /|\\\n"
"    (( )\\/:  :\\/(( ))\n"
"     \\\\//\\:  ://\\\\//\n"
"      \\/\\\\:  ://\\/\n"
"       \\ \\\\:;//\\ \\\n"
"       /\\ \\;// /\\ \\\n"
"      /  \\/\\\\// \\  \\\n"
"     /    \\;/     \\  \\\n"
"    /     //\\\\     \\  \\\n"
"   /     //  \\\\     \\  \\\n"
"  /     //    \\\\     \\  \\\n"
" /     //      \\\\     \\  \\\n"
"/     //        \\\\     \\  \\\n"
"/     //          \\\\     \\  \\\n";

const QString DRAGON_AA = 
"                                        ___====-_  _-====___\n"
"                                  _--^^^#####//      \\\\#####^^^--_\n"
"                               _-^##########// (    ) \\\\##########^-_\n"
"                              -############//  |\\^^/|  \\\\############-\n"
"                            _/############//   (@::@)   \\\\############\\_\n"
"                           /#############((     \\\\//     ))#############\\\n"
"                          -###############\\\\    (oo)    //###############-\n"
"                         -#################\\\\  / VV \\  //#################-\n"
"                        -###################\\\\/      \\//###################-\n"
"                       _#/|##########/\\######(   /\\   )######/\\##########|\\#_\n"
"                       |/ |#/\\#/\\#/\\/  \\#/\\##\\  |  |  /##/\\#/  \\/\\#/\\#/\\#| \\|\n"
"                       `  |/  V  V  `   V  \\#\\| |  | |/#/  V   '  V  V  \\|  '\n"
"                          `   `  `      `   / | |  | | \\   '      '  '   '\n"
"                                           (  | |  | |  )\n"
"                                          __\\ | |  | | /__\n"
"                                         (vvv(VVV)(VVV)vvv)\n";



const QString VENDOR_AA = 
"_____\n"
"         _|[]_[]|_\n"
"        |_________| \n"
"       /=====_======/\\\n"
"     /    ((\\o/))     \\\n"
"    /      /\\_/\\       \\\n"
"   /  /|  ((-_-))   |\\   \\\n"
"  /  / |\\/\\)___(/\\/ | \\   \\\n"
" /  /  |:::/^\\::::::|  \\   \\\n"
"|  |   |::::::::::::|   |  |\n"
"|__|   |::::::::::::|   |__|\n"
"|  |   |::::::_:::::|   |  |\n"
"|  |   |:::::(_)::::|   |  |\n"
"|  |___|::::::::::::|___|  |\n"
"|  |===|::::::::::::|===|  |\n"
"|  |   |:::::::::::-|   |  |\n"
"|  |   |:::::::::::(|   |  |\n"
"|  |   |::::::::::::|   |  |\n"
"|__|___|::::::::::::|___|__|\n"
"|__|===|::::::::::::|===|__|\n"
"   |   |::::::;;::::|   |\n"
"   |   |:::::;:;::::|   |\n"
"   |   |:::::;:;::::|   |\n"
"   |___|::::::;;::::|___|\n"
"   |===|::::::::::::|===|\n"
"   |\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"|\n"
"   |\"\"\"\"\"\"SHOP\"\"\"\"\"\"\"\"\"\"|\n"
"   |\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"|\n"
"  /                     \\\n"
" /_____________________()\\\n";

#endif

}

#endif
