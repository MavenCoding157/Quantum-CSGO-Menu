#include "Discord.h"

void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("1150086462142943302", &Handle, 1, NULL);
}

void Discord::Update()
{

        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state = "Quantum CSGO Menu";
        discordPresence.details = "Undetected CSGO External Cheat";
        discordPresence.startTimestamp = 1507665886;
        discordPresence.endTimestamp = 1507665886;
        discordPresence.largeImageKey = "final";
        discordPresence.largeImageText = "Quantum CSGO Menu";
        discordPresence.smallImageKey = "final";
        Discord_UpdatePresence(&discordPresence);
 
}

