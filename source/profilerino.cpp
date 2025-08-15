#include <unordered_map>
#include <string>
#include <vitasdk.h>

static std::unordered_map<uint32_t, std::string> sym_table;
static uint8_t sym_table_ready = 0;

extern "C" {
	
int sceRazorCpuPushMarkerWithHud(const char* label, int color, int flags);
int sceRazorCpuPopMarker();

void  __cyg_profile_func_enter(void *this_fn, void *call_site) {
	if (sym_table_ready && sym_table.find((uint32_t)this_fn) != sym_table.end()) {
		sceRazorCpuPushMarkerWithHud(sym_table[(uint32_t)this_fn].c_str() , 0x8000ffff, 0);
	} else {
		char addr[32];
		sprintf(addr, "func %x", this_fn);
		sceRazorCpuPushMarkerWithHud(addr, 0x8000ffff, 0);
	}
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
	sceRazorCpuPopMarker();
}

void profilerino_init() {
	sceSysmoduleLoadModule(SCE_SYSMODULE_PERF);
	sceClibPrintf("profilerino: Initing...\n");
	
	// Load sym table (nm --without-symbol-versions --demangle FILE.ELF > sym.txt)
	char *buf;
	sceClibPrintf("Loading sym table\n");
	FILE *f = fopen("app0:/sym.txt", "rb");
	if (!f) {
		sceClibPrintf("profilerino: Missing sym table, falling back to address markers\n");
		return;
	}
	fseek(f, 0, SEEK_END);
	size_t sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = (char *)malloc(sz + 1);
	fread(buf, 1, sz, f);
	buf[sz] = 0;
	fclose(f);
	
	char *startline = buf;
	char *endline = strstr(startline, "\n");
	uint32_t addr;
	char dummy;
	char func_name[2048] = {};
	while (endline) {
		endline[0] = 0;
		if (*(endline - 1) != ' ' && !strstr(startline, " w ")) { // Ignore missing function names and addresses
			sscanf(startline, "%x %c %s", &addr, &dummy, func_name);
			//sceClibPrintf("%x -> %s\n", addr, func_name);
			sym_table[addr] = func_name;
		}
		startline = endline + 1;
		endline = strstr(startline, "\n");
	}
	
	free(buf);
	sym_table_ready = 1;
	sceClibPrintf("profilerino: Sym table inited!\n");
}

}