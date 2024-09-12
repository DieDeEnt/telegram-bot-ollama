#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#define SQLITECPP_COMPILE_DLL
#include <SQLiteCpp/SQLiteCpp.h>
//#include <vector>
#include <sqlite3.h>
#include <ctime>
#include <time.h>
#include <fstream>
//#include <SFML/Graphics.hpp> 
#include <random> 
#include <cctype>
#include <ollama/ollama.hpp>

using namespace SQLite;
using namespace std;
using namespace TgBot;
using namespace StringTools;


// open db
Database db("data/db/Database.db", OPEN_READWRITE | OPEN_CREATE);

static int week()
{
    time_t t = time(NULL);
    tm* now = localtime(&t);
    return now->tm_wday;
}

ChatMember::Ptr UserInfo(Bot& bot, Message::Ptr message)
{
    ChatMember::Ptr ChatMemberUser = bot.getApi().getChatMember(message->chat->id, message->from->id);
    return ChatMemberUser;
}


int main()
{
    setlocale(LC_ALL, "RU");
    srand(time(NULL));

    Bot bot("6335241580:AAEfid9JQ8f2hK5vBMDT59MAe1KNNIHdojw");

    //start@GachiTonoKaiBot
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) 
    {
       // switch (UserInfo(bot, message)->user->id)
       // {
       // case 1732616250:
            
       //     bot.getApi().sendMessage(message->chat->id, u8"no");
      //      break;
      //  case 1643155787:
      //      bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8"no");
      //      break;
     //   case 987626057:
     //       bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8"no");
      //      break;
      //  case 5089396169:
      //      bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8" ");
       //     break;
     //   default:
      //      bot.getApi().sendMessage(message->chat->id, u8" " + UserInfo(bot, message)->user->firstName + u8" ");
      //      break;
    //    }
        try 
        {
            // Open db
            SQLite::Database database("data/db/Database.db", SQLite::OPEN_READWRITE);

            // Select column
            SQLite::Statement query(db, "SELECT COUNT(*) FROM users WHERE user_id = ?");

            //
            query.bind(1, UserInfo(bot, message)->user->id);

            // 
            if (query.executeStep()) 
            {
                int count = query.getColumn(0).getInt();

                //
                if (count > 0) 
                {
                    cout << "User " << UserInfo(bot, message)->user->username << " with id " << UserInfo(bot, message)->user->id << " has already been logged into the database." << endl;
                }
                else 
                {
                    cout << "[NEW!!!!!] User " << UserInfo(bot, message)->user->username << " with id " << UserInfo(bot, message)->user->id << " has not been found and has been successfully logged into the database." << endl;
                    bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8" ");

                    // 
                    SQLite::Statement query(db, "INSERT INTO users(user_id, username, user_name, user_surname) VALUES(?1, ?2, ?3, ?4)");
                    query.bind(1, UserInfo(bot, message)->user->id);
                    query.bind(2, UserInfo(bot, message)->user->username);
                    query.bind(3, UserInfo(bot, message)->user->firstName);
                    query.bind(4, UserInfo(bot, message)->user->lastName);
                    query.exec();
                }
            }
        }
        catch (const std::exception& e) 
        {
            // 
            std::cout << "Error: " << e.what() << std::endl;
        }
	


    });



    bot.getEvents().onAnyMessage([&bot](Message::Ptr message)
    {

	
 	bot.getApi().sendMessage(message->chat->id, ollama::generate("qwen2:0.5b", message->text));





    });





    // main bot cycle
    try 
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        cout << "Nomber week " << week() << endl;
        TgLongPoll longPoll(bot);
        printf("Bot activated\n");
        while (true) 
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgException& e) 
    {
        printf("error: %s\n", e.what());
    }

    return 0;   
}

