bool Overload(int lastMessage, int nowMessage){

    bool much = false;

    if (nowMessage - lastMessage < 5)
    {
        much = true;
    }
    


    return much;
}
