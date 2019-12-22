
void
info_modules()
{
    MODULEENTRY me;

    if (!ModuleFirst(&me))
    {
	fprintf(debug_outfp, "No modules.\n");
	return;
    }

    fprintf(debug_outfp, "HANDLE NAME                 SIZE     EXE PATH\n");

    do
    {
	fprintf(debug_outfp, "%04x   %-20s %08x",
		me.hModule, me.szModule, (unsigned int) me.dwSize);
	if (me.szExePath[0])
	    fprintf(debug_outfp, " %40s\n", me.szExePath);
	else
	    fprintf(debug_outfp, "\n");

	me.szExePath[0] = '\0';
    }
    while (ModuleNext(&me));
}

