#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <QString>

namespace AsciiArt {

const QString ENTRANCE_AA = R"(
    _____________________
   /                     \
  /     ___________       \
 /     |  _______  |       \
|      | |       | |        |
|      | |       | |        |
|      | |       | |        |
|      | |     _ | |        |
|      | |    |_|| |        |
|      | |     __| |        |
|      | |    |  | |        |
|      | |    |  | |        |
|      | |()  |  | |        |
|      | |()  |  | |        |
|      | |()  |  | |        |
|      |_|()__|__| |        |
|     /           \ |       |
|    /             \|       |
|   /               \       |
|  /                 \      |
| /                   \     |
|/                     \    |

)";

const QString STAIRSUP_AA = R"(

⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠰⡇⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣠⠽⠶⣞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢠⠚⣡⠀⠀⢎⠓⠤⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡞⢸⣿⠀⠀⢸⠉⠓⠲⠾⠃⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠓⠚⣿⠀⠀⣸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⡏⢀⠦⣄⠉⢲⠀⠀⠀⠀⣶⠒⠒⠒⠒⢶
⠀⠀⠀⠀⠀⠀⠀⣸⠁⣸⠠⢸⠁⢸⢠⠶⠶⠴⣿⠀⠀⠀⠀⢸
⠀⠀⠀⠀⠀⠀⢡⠇⢠⠋⠀⣬⣷⣾⣼⡇⠈⠀⠁⠁⠀⠀⣠⠾
⠀⠀⠀⠀⠀⠀⠈⢓⣋⣀⣀⡇⠀⠀⠀⠀⠀⠀⢀⣠⠔⠋⠀⠀
⠀⠀⠀⢀⡀⠀⠀⣸⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠋⠀⠀⠀⠀⠀
⠀⠀⠀⢸⡍⠉⠉⠉⠀⠀⠀⠀⢀⡠⠖⠉⠀⠀⠀⠀⠀⠀⠀⠀
⡏⠉⠉⠉⠁⠀⠀⠀⠀⢀⡤⠚⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⡇⠀⠀⠀⠀⠀⣀⡴⠚⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠓⠒⠒⠒⠒⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
)";

const QString STAIRSDOWN_AA = R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣶⣿⣿⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⡿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣏⠀⣿⣿⣿⣿⣿⣿⣷⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡗⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⠷⠀⣿⣿⣿⣿⣿⣿⠀⠈⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠀⠀⣝⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣯⣻⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣷⣝⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⠀⠀⠀⠀⢀⣠⣴⣿⣿⣿⣿⡿⠙⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⢠⣶⣿⣿⣿⣿⡿⠛⠁⠀⠀⠸⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠹⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠀⢻⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⠀⠀⠀⠀⠈⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⣿⣿⠀⠀⠀⠀⠀⠸⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣀⣀⣀⣀⣀⣀⣙⣟⣋⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠿⠿⠿⠿⠿⠿⠿⠿⢿⢿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉
)";

const QString POOL_AA = R"(
         .*.         
      .*.*.*.*.      
    .*.* ___ *.*.    
  .*.*  (   )  *.*.  
.*.*    (   )    *.*.
*:*  ___\   /___  *:*
*:* (           ) *:*
*:* )  ~~~ ~~~  ( *:*
*:* ( ~~~~~~~~~ ) *:*
*:* )  ~~~ ~~~  ( *:*
*:* (  ~~~~~~~  ) *:*
*:* ) ~~~~~~~~~ ( *:*
*:* (___________) *:*
 *:*    \___/    *:* 
  *:*           *:*  
   *:*.*.*.*.*.*:*   
     '*.*.*.*.*'     
)";

const QString CHEST_AA = R"(
          ____...------------...____
       _.-"` /o/__ ____ __ __  __ \o\_`"-._
     .'     / /                    \ \     '.
     |=====/o/======================\o\=====|
     |____/_/________..____..________\_\____|
     /   _/ \_     <_o#\__/#o_>     _/ \_   \
     \_____________\##########/_____________/
      |===\!/========================\!/===|
      |   |=|          .---.         |=|   |
      |===|o|=========/     \========|o|===|
      |   | |         \() ()/        | |   |
      |===|o|======{'-.) A (.-'}=====|o|===|
      | __/ \__     '-.\uuu/.-'    __/ \__ |
      |============== .'.'^'.'.============|
      |  _\o/   __  {.' __  '.} _   _\o/  _|
      `""""-""""""""""""""""""""""""""-""""`
)";

const QString CRYSTALORB_AA = R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣤⡤⡤⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡴⣺⡛⣛⡿⠟⠛⢹⡯⣩⠍⢙⡯⢟⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⠟⣗⣉⣿⣗⣿⡗⢄⠀⠚⠓⠦⠤⠛⣮⢾⣅⢹⡋⠙⠷⣄⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⢞⣑⣲⣛⣿⣿⣿⣼⠟⠛⠒⠱⠀⠀⠀⠀⠀⠉⢿⣋⣷⡜⡆⠀⠈⢻⣢⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⡟⠛⠛⠀⠀⠀⠘⢣⡀⠀⠘⣵⢽⢆⠀⠀⠑⠷⣁⢷⣬⣓⣆⠀⠙⡷⡄⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⢿⣿⡿⣿⠀⠀⠀⠀⠀⠀⠀⠈⠁⢀⡀⢁⣮⣋⡻⡢⠀⠰⣊⡉⠘⢯⡮⣠⡄⠈⣿⡄⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢠⣿⡟⢯⣯⣉⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠖⠷⠎⢩⡍⠁⠀⠀⠘⣍⠧⢾⡾⡬⣻⣤⠸⣿⡄⠀
⠀⠀⠀⠀⠀⠀⠀⣾⡿⠁⠀⠹⣟⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣢⡀⠀⠀⠀⢀⡝⠏⣇⠸⣿⣨⣨⣆⣿⣷⠀
⠀⠀⠀⠀⠀⠀⢀⣿⢷⡀⠀⡀⠩⠹⡯⢷⢤⣀⣤⢀⣀⢤⣄⣀⠠⣴⡟⠛⢣⡀⠀⢀⣼⣥⣿⡿⠦⠾⠚⠀⢸⡁⣻⡄
⠀⠀⠀⠀⠀⠀⢸⣿⡯⢝⣶⠁⠀⠀⠈⠛⢯⢿⠿⢿⣒⣦⣤⣦⣦⣦⠛⠻⣇⣸⣓⡿⣾⣯⣦⢧⣦⠠⠄⣀⢼⣵⢼⡇
⠀⠀⠀⠀⠀⠀⠸⣿⢇⢿⣍⣇⠀⠀⠀⣠⣤⠈⠀⠀⠈⠙⠫⠽⢟⣱⢻⣦⡛⣯⣿⠧⠞⠱⠷⠀⠀⠀⠸⠝⢾⣟⣿⡇
⠀⠀⠀⠀⠀⠀⠀⣿⢷⣤⣛⣶⣆⠀⠈⠉⠉⠀⠀⣤⠀⠀⢀⡀⠀⠀⣉⡺⠚⠂⠋⠐⣶⣤⣀⣤⡀⣠⡀⠐⠻⠾⣾⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣟⢓⢻⡟⠽⣵⠄⠀⠀⠀⠀⠀⠀⢀⣼⢿⣷⠀⠉⠙⢤⣄⠀⠸⠾⣾⣿⣿⡿⣿⠃⠀⠀⣰⡏⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢻⡏⠋⠫⣷⢻⡧⣦⠀⠀⠀⢀⠀⠙⢯⣯⡈⠶⠗⠀⠳⣷⣶⣖⣵⣿⣿⠭⠛⠃⣀⡀⢠⡟⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣆⠀⠀⠀⢠⣬⠁⣤⣀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠈⠻⠟⠛⠋⠀⠀⠠⣺⠿⣣⡟⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⢆⠀⠈⠚⣆⠼⢷⠿⣥⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⣠⣤⡝⢀⡴⠋⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠀⠀⠀⢀⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢻⣶⣷⣶⣶⣤⡀⠀⡀⣤⣄⡀⠀⢀⣠⣴⣶⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⡆⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣽⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⡻⣿⣿⠁⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⣿⣿⣿⡿⠿⠛⠛⠛⠿⠿⠿⠟⠛⠛⠛⠛⢿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⡄⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣷
)";

const QString BOOK_AA = R"(
                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
            __.'              ~.   .~              `.__
          .'//                  \./                  \\`.
        .'//                     |                     \\`.
      .'// .-~\"\"\"\"\"\"\"~~~~-._     |     _,-~~~~\"\"\"\"\"\"\"~-. \\`.
    .'//.-\"                 `-.  |  .-'                 \"-.\\`.
  .'//______.============-..   \ | /   ..-============.______\\`.
.'______________________________\|/______________________________`.
)";

const QString KOBOLD_AA = R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢶⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⡲⢶⣶⣶⣶⣿⣯⣷⣏⡿⡴⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⢿⣿⣜⢧⢯⡹⣝⠾⣹⢏⡿⣻⠿⣿⣿⣿⣿⣶⣝⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣨⣿⣿⣮⡳⡝⡾⣱⢫⡜⣧⢻⡱⣏⢿⣿⣿⣿⣿⣷⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⣹⣿⣿⣿⣏⣳⣭⢳⡝⣎⣷⣹⣮⣷⣽⣻⣿⣷⣿⣻⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣿⣿⣿⣿⣿⣿⣳⣟⣾⣽⣻⡾⠿⢛⣻⣟⢿⣿⣻⣿⣿⣿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠻⣿⣿⣟⡿⣯⣟⡳⢞⡱⢮⠏⠀⠀⣸⢿⣿⣦⠙⣭⢿⣿⣟⣿⣻⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣽⣿⢻⡼⢭⣏⡽⡏⠀⠀⠸⣧⣼⣿⣾⡆⢩⡞⣭⢻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⢿⣏⢮⣳⢚⢧⣞⣱⡇⠀⠀⠀⢿⢾⣿⣯⠇⢸⡝⣮⢳⠮⣝⡻⢿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣿⢻⡝⣮⢳⢎⡷⢎⡻⢯⡝⢯⣻⣄⠀⠀⠈⠛⠮⢙⢀⡾⣹⢎⣗⡻⡼⣹⣟⣽⠿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣾⡵⣏⣾⣜⣫⡝⢧⣿⡻⣵⢻⢷⣥⣤⣤⣤⡴⣯⢳⡝⣺⢬⡳⣝⡱⢯⢿⣟⢮⡟⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣿⣷⣿⣶⣯⣷⣺⣽⢞⣧⠿⣬⣯⣷⣹⣜⡣⢏⣞⢱⣯⡜⣧⢻⡹⣞⡻⢮⡝⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠛⢹⣷⣯⣟⡷⣿⣯⠿⠿⢷⡾⠾⠷⠾⠷⠾⠾⢧⣮⣿⣿⣱⡿⣶⣹⢣⢿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⢯⡿⣳⣿⣩⣛⣽⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠤⠤⣀⠀⠛⣿⠽⣚⣓⣉⣉⣉⣉⣩⠩⢔⣒⡠⠀⠀⠀⠀⠀⠀⠀⢰⣶⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡰⠁⢠⣤⠟⢹⡽⣶⣉⡓⠒⡒⠆⠰⠄⠀⠀⠉⠉⠓⡀⠀⠀⠀⢀⣴⣀⣀⡹⣿⣿⣿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⡔⠊⠀⠘⠁⢎⣇⣾⣷⣖⣦⡴⣤⠴⣒⢀⣤⣄⣠⠄⢠⡄⢣⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⠤⢦⡽⢒⡥⡿⠁⠞⠛⠮⣅⠒⡬⠨⡺⣷⠸⣇⣾⡧⡛⠀⠀⠀⢻⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠒⠛⡞⣁⡢⡖⢀⣤⣾⢡⠂⠀⠀⠀⠀⠀⢷⠿⢽⡇⠀⠀⠀⣷⢻⡟⡿⣟⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⡟⣿⣿⡯⣿⡷⠀⠀⠀⠀⠀⠀⠀⠀⠞⣄⠀⠀⡐⣯⣻⡽⣽⣻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢼⣿⡱⢿⡐⠁⠐⡚⠤⡀⠀⠀⠀⠀⡸⣷⢶⡱⢌⠏⢻⢗⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣅⣿⣿⣹⢻⡑⠤⡄⢀⡀⢆⣀⠠⠐⣔⣾⣿⢯⡷⣦⣽⣬⠅⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢠⣤⣤⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢾⣟⠛⠪⠽⢊⣲⠥⣇⡌⠀⢀⣠⡮⢙⡊⡽⠯⠟⠛⠉⣸⠃⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠘⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣤⣤⣤⣤⣄⣠⡪⠳⠓⣈⠥⡐⢸⣯⣿⣿⣿⣿⣯⠿⡑⠲⣜⢵⠀⠀⠀⠀⠎⡘⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠛⠛⠛⠛⠻⢿⡿⠾⢿⡷⢂⠬⡒⢁⠔⣵⣿⣶⣷⣦⣤⣤⣤⣤⣤⣥⣼⣤⣤⣤⣤⣤⣥⣀⣠⣤⣤⡠⣔⠶⠊⠑⡺⠁⠆⡤⡀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣦⣤⣅⡀⠘⠞⣄⠈⡌⠨⣿⡟⠉⠉⠉⠉⠉⠉⠙⡻⢻⠻⢟⠛⠛⠛⠛⠓⢿⣿⣿⣿⠗⡠⠡⣠⠞⠁⠀⠀⠀⣁⡋⢖⣤
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣶⣾⣿⣿⣿⣽⣯⣿⢀⡄⠀⣨⠉⠉⠁⢡⠀⢀⠎⠀⠀⠀⢈⣤⣧⠁⣹⡄⠀⠀⠀⠀⠀⠀⠉⠉⠈⠱⠇⣴⣆⡄⣶⢩⠿⠶⠈⠁⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣰⣿⣿⣿⣿⣿⣿⣽⣯⣿⣿⣿⣿⠟⢣⠞⠁⢀⢠⢠⣿⢀⠎⠀⠀⢰⠆⢨⡏⣵⣿⡟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠉⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣀⣤⣤⣼⣿⠿⣟⢯⡳⣝⢮⣳⢹⣫⣿⣻⣞⡏⣂⣠⣴⣶⣾⣶⣾⣿⣼⣶⣄⠀⡀⡤⢊⢠⣽⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢤⣤⡔⣶⢻⢿⡹⣏⡟⡼⢎⡟⣼⢣⡝⣮⡷⣞⡿⣽⣞⡷⣯⣟⡿⣿⣻⣞⣷⣻⣟⡾⣽⢯⣷⣧⣿⡿⣷⣶⣟⣞⣻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠉⠛⠼⢯⣾⣵⣳⠾⣽⣽⣺⢷⣻⣟⣷⣻⡽⢻⠷⣏⣟⠭⢗⣺⣿⣳⣟⣾⠳⣏⠾⣥⣛⠶⣻⣯⣟⣷⣻⢾⡼⣹⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠉⠉⠛⠛⠋⠭⠯⠧⠷⠶⠥⠭⠯⠛⠒⠛⠉⠁⣼⣯⢷⡾⣭⢻⡜⣏⠶⣭⣛⡽⣷⣻⡾⣽⣻⢾⣵⣳⡳⣶⣤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣟⣯⣿⣽⣻⡽⣞⡿⣶⣣⣽⣶⣷⣿⣿⣿⣿⣾⣓⠛⠛⠛⠛⠛⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠛⠛⠛⠓⠛⠿⠿⠿⠿⠿⠿⠿⠛⠛⠃⠀⠀⠀⠀⠀⠀
)";

const QString ORC_AA = R"(
⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⡂⠢⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡒⣄⣼⣿⣟⠿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠱⡀⠡⡀⢂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠼⠛⠛⣯⣿⣿⡲⢉⡣⣧⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠘⣁⠒⡄⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡔⠈⢡⠊⠉⠣⣠⠤⣵⣾⣿⣿⣍⢗⣻⣜⢅⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠘⠖⠨⣆⠐⡀⠀⠀⠀⠀⠀⠀⠀⢀⢼⣧⣈⡤⠂⠥⢀⠗⠢⢁⠛⢿⣿⣿⣞⣶⣴⣾⣿⡦⠄⠀⠀⠀⠀⠀
⠀⠀⠀⠈⢇⠱⡣⠀⠄⠀⠀⠀⢀⠄⠐⠊⡜⡟⠿⣿⣷⣶⠟⣀⡚⢀⠂⠌⢿⣿⣿⣿⣿⣿⣿⢇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢣⠙⣕⡀⢂⠀⢠⣼⣶⣧⡴⣱⣧⣃⣨⣿⣿⣯⡁⠐⡀⠊⢄⣼⣿⣿⣿⣿⣻⠙⣑⠄⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢹⡈⣎⣄⣴⣿⣿⣿⣿⣶⣿⠉⠁⢹⣿⣿⣿⣿⣷⣶⣿⣿⣿⣷⡟⣫⣼⣿⣧⣧⣵⣷⡞⠀⠀⠀
⠀⠀⠀⠀⢠⣄⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⢫⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠸⣿⣿⣿⣿⣿⣟⠃⠀⠀
⠀⠀⠀⠀⠀⠻⣿⠛⠁⠻⠟⠿⡿⢿⠿⠁⠀⠀⠀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⠀⠀⠜⠛⢿⣿⣿⣿⡄⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⡁⡔⡀⢤⣧⠀⠀⢀⡾⣿⣿⣿⣿⣿⣿⣿⣿⣻⣣⣿⣿⣷⣦⣀⠀⠀⣻⡏⠠⠬⠢⡀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢿⣾⣿⡇⠀⢠⣾⢿⣳⣛⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣟⣿⣶⣄⠹⣇⡵⣷⣶⡆
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣷⡤⡟⣼⣣⢯⡝⣾⣽⢿⣿⣿⣿⣿⣿⣿⣿⣳⡭⣿⣣⣟⣵⠉⠁⠴⠟⠁
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣩⣶⣿⣿⣿⣧⡻⠂⠀⢸⣿⠟⢹⣿⠁⠀⣩⣿⣿⣷⣿⡿⠏⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣽⣿⣿⣿⣿⠏⠀⠀⠀⠈⠁⠀⠈⠇⠀⠐⣿⣿⣿⣿⡛⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⡿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣧⣀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣶⣆⡀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠛⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀
)";



const QString BALROG_AA = R"(
                )  (
             (   ) )
              ) ( (
     /\  .-"""-.  /\
    //\\/  ,,,  \//\\
    |/\| ,;;;;;, |/\|
    //\\\;-"""-;///\\
   //  \/   .   \/  \\
  (| ,-_| \ | / |_-, |)
    //`__\.-.-./__`\\
   // /.-(() ())-.\ \\
  (\ |)   '---'   (| /)
   ` (|           |) `
     \)           (/
      |    )(    |
     /|\  /--\  /|\
    (( )\/:  :\/(( ))
     \\//\:  ://\\//
      \/\\:  ://\/
       \ \\:;//\ \
       /\ \;// /\ \
      /  \/\\// \  \
     /    \;/     \  \
    /     //\\     \  \
   /     //  \\     \  \
  /     //    \\     \  \
 /     //      \\     \  \
/     //        \\     \  \
/     //          \\     \  \
)";

}

#endif
