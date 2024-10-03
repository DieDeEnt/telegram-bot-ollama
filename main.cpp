#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#define SQLITECPP_COMPILE_DLL
#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>
#include <ctime>
#include <time.h>
#include <fstream>
#include <random> 
#include <cctype>
#include <ollama/ollama.hpp>

//Sub include
#include "sub/func.cpp"
#include "config.cpp"

using namespace SQLite;
using namespace std;
using namespace TgBot;
using namespace StringTools;
using namespace ollama;

// open db
Database db("Database.db", OPEN_READWRITE | OPEN_CREATE);




//set time
static int week()
{
    time_t t = time(NULL);
    tm* now = localtime(&t);
    return now->tm_wday;
}


//create user object
ChatMember::Ptr UserInfo(Bot& bot, Message::Ptr message)
{
    ChatMember::Ptr ChatMemberUser = bot.getApi().getChatMember(message->chat->id, message->from->id);
    return ChatMemberUser;
}



const vector <string> cmd =
{
    u8"/start",
    u8"/share",
    u8"/schedule_all",
    u8"/schedule",
};


int main()
{
    setlocale(LC_ALL, "RU");
    srand(time(NULL));

    Bot bot("6335241580:AAEfid9JQ8f2hK5vBMDT59MAe1KNNIHdojw");

    //start@GachiTonoKaiBot
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) 
    {
        bot.getApi().sendChatAction(message->chat->id, "typing");
        //check db
        try 
        {
            // Open db
            Database database("Database.db", SQLite::OPEN_READWRITE);

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
                    cout << "User " << UserInfo(bot, message)->user->firstName << " with id " << UserInfo(bot, message)->user->id << " has already been logged into the database." << endl;
                    
                    bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8", я тебя уже знаю");
                }
                else 
                {
                    cout << "[NEW!!!!!] User " << UserInfo(bot, message)->user->firstName << " with id " << UserInfo(bot, message)->user->id << " has not been found and has been successfully logged into the database." << endl;
                    
                    bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8", теперь ты в базе, бойся");

                    // write user
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
            // print error
            std::cout << "Error: " << e.what() << std::endl;
        }
	


    });

    bot.getEvents().onCommand("share", [&bot](Message::Ptr message) 
    {
        bot.getApi().sendChatAction(message->chat->id, "typing");
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/cringe.jpg", "image/jpg"));
        bot.getApi().sendChatAction(message->chat->id, "typing");
        bot.getApi().sendMessage(message->chat->id, u8"Это бот одного моего знакомого скуфа-хики t.me/slaveforleatherbags_bot, ");
    });
    
    //Create Photo
    // bot.getEvents().onCommand("cp", [&bot](Message::Ptr message)
    // {
    //         try
    //         {
    //             string size = message->text.substr(4);
    //             stoi(size);
    //             cout << "User write size " << size << endl;
    //             if (stoi(size) > 3000 || stoi(size) <= 0)
    //             {
    //                 bot.getApi().sendMessage(message->chat->id, u8"������ �� ������ ���� �� ���������, ��������� ����������� ��������:");
    //                 bot.getApi().sendMessage(message->chat->id, u8"���� ������ �������� ������ ����, ����������� ��������� ������ ������� - /cp [������ ����]");
    //                 int size = 300;
    //                 paint(size);
    //                 return 0;
    //             }
    //             paint(stoi(size));
    //         }
    //         catch (const std::exception&)
    //         {
    //             bot.getApi().sendMessage(message->chat->id, u8"������ �� ������ ���� �� ���������, ��������� ����������� ��������:");
    //             bot.getApi().sendMessage(message->chat->id, u8"���� ������ �������� ������ ����, ����������� ��������� ������ ������� - /cp [������ ����]");
    //             int size = 800;
    //             paint(size);
    //         }
    //         bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("Photo/RandomPhoto.jpg", "image/jpg"));
    // });
    
    bot.getEvents().onCommand("schedule_all", [&bot](Message::Ptr message) {
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/1.jpg", "image/jpg"));
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/2.jpg", "image/jpg"));
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/3.jpg", "image/jpg"));
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/4.jpg", "image/jpg"));
        bot.getApi().sendChatAction(message->chat->id, "upload_photo");
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/5.jpg", "image/jpg"));
    });
    
    // bot.getEvents().onAnyMessage([&bot](Message::Ptr message) 
    // {
    //     if (message->document != nullptr) {
    //         string fileId = message->document->fileId;
    //         File::Ptr file = bot.getApi().getFile(fileId);
    //         const std::string& url = "https://api.telegram.org/file/bot" + bot.getToken() + "/" + file->filePath;
    //         string fileName = message->document->fileName;
    //         std::ofstream outfile("downloads/" + fileName, ofstream::binary);
    //         if (outfile.is_open()) 
    //         {
    //             outfile << url;
    //             bot.getApi().downloadFile(url);
    //             bot.getApi().sendMessage(message->chat->id, "File downloaded successfully!");
    //             bot.getApi().sendMessage(message->chat->id, "Here is the link to download your file " + url);
    //             cout << message->chat->username << " File downloaded successfully!" << std::endl;
    //         }
    //         else {
    //             cerr << "Error opening file for writing." << std::endl;
    //         }
    //     }
    // });

    //schedulenow
    bot.getEvents().onCommand("schedule"  , [&bot](Message::Ptr message)
    {
        bot.getApi().sendChatAction(message->chat->id, "typing");
        switch (week())
        {
        case 1:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Сегодня понедельник. Вот твое расписание");
            bot.getApi().sendChatAction(message->chat->id, "upload_photo");
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/1.jpg", "image/jpg"));
            break;
        case 2:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Сегодня вторник. Вот твое расписание");
            bot.getApi().sendChatAction(message->chat->id, "upload_photo");
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/2.jpg", "image/jpg"));
            break;
        case 3:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Сегодня среда. Вот твое расписание");
            bot.getApi().sendChatAction(message->chat->id, "upload_photo");
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/3.jpg", "image/jpg"));
            break;
        case 4:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Сегодня четверг. Вот твое расписание");
            bot.getApi().sendChatAction(message->chat->id, "upload_photo");
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/4.jpg", "image/jpg"));
            break;
        case 5:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Сегодня пятница. Вот твое расписание");
            bot.getApi().sendChatAction(message->chat->id, "upload_photo");
            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("data/schedule/5.jpg", "image/jpg"));
            break;
        case 6:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Зачилься на кондициях, сегодня суббота");
            break;
        case 0:
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, u8"Бро, сегодня воскресенье, напряги булки на компициях");
            break;
        default:
            //Error
            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(1732616250, u8"Как ты это смог сломать, гений?");
            return;
        }
    });

    //Send message ollama
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message)
    {
        for (int j = 0; j < cmd.size(); j++){if (message->text == cmd[j]){return;}}
        if (message->text.empty()) 
        {
            bot.getApi().sendMessage(message->chat->id, UserInfo(bot, message)->user->firstName + u8", это не текст!");

        } else {

            cout << UserInfo(bot, message)->user->firstName << " told the bot: " << '"' << message->text << '"' << endl;

            bot.getApi().sendChatAction(message->chat->id, "typing");
            bot.getApi().sendMessage(message->chat->id, ollama::generate(MODEL, message->text));

            cout << "The response has been sent successfully" << endl;
        }
    });

    //main bot cycle
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