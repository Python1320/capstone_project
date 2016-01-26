#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/board.h>
#include <arch/board/board-reset.h>

#include <stdio.h>
#include <string.h>


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int resetdfu_main(int argc, char *argv[])
#endif
{
	board_reset_to_system_bootloader();
    return 0;
}
