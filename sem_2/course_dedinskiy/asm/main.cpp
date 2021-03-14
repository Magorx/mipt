extern "C" void k_printf(char *str, ...);
extern "C" int  k_sum_wrap(int a, int b);

extern "C" int k_sum(int a, int b) {
	return a + b;
}

int main() {
	k_printf("%x\n", -123);
	k_printf("hi %man | smb[%c] str[%s], %d, %d, %d, %d, %d, %d, %d, %x, %b\n\nAnd I %s %x%d%%%c%b", '$', "alalalalala", k_sum_wrap(1, 10), 2, 3, 4, 5, 6, 7, -8, -9, "LOVE", 3802, 100, 33, 255);

	return 0;
}

/*valgrind 
callgrind
~
kcash */