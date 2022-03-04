void recieve_tmp(Observed<int> a) {
    FuncLogger flg("recieve_tmp");
    printf("lol %d\n", a.get_data());
}

Observed<int> return_tmp() {
    FuncLogger flg("return_tmp");

    OBSERVED(int, a, 1);
    return a + a;
}


void func(int = 2) {
    FuncLogger flg("func");

    OBSERVED(int, a, 1);
    OBSERVED(int, b, 99);

    a = return_tmp();
}