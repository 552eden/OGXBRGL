# OGXBRGL - Play OG Xbox Games games on RGLoader with Proto "support"

An RGLoader plugin designed to re-enable memory protection on launch of OGXbox games. also shuts down Proto so it wont crash the console

This plugin uses Byrom's Halo Sunrise 2 plugin as a template. original code can be found in: https://github.com/Byrom90/Halo_Sunrise_Plugin_2.0

Guide:
 - Place OGXBRGL.xex on root of hdd (or folder of your choice).
 - Configure OGXBRGL.xex as plugin 1 (Optional, configure Proto.xex as plugin 2).
 - Before launching og xbox games first wait for proto to complete authentication (either "Ready to connect to Xbox Live or Authentication failed messages).
 
 Current limitations:
 - if "Replace faimily settings with jump to XShell is enabled in RGLoader, attempting to open the guide will crash the console.
 - Proto cannot be reloaded after exit of OGXbox games
 - Basically, to exit the OGXbox game you have to shutdown the console. maybe ill fix this in the future, maybe not.
 - if this is not working for you, check the logs and let me know but you probably need to change an address in your RGLoader image. see RGLoader patching instructions
 
 RGLoader patching instructions:
 - inside RGLoader-17489_HvxPeekPoke.S:
	li        %r5, 0x154C
	this is be the wrong address(for 17489). change it to 0x15F0 and it should work
	(Thanks for this em, this is the base for the whole project)!

Credits:
 - Byrom - original Halo Sunrise 2 plugin that this project uses as a template
 - craftycodie - Halo hooks and addresses and original sunrise-plugin
 - FreestyleDash Team - xkelib
 - Xbox 360 Hub #coding-corner - yall are the best!