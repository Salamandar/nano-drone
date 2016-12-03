MEMORY
{
  rom  (rw)  : ORIGIN = 0x08000000, LENGTH = 12K
  data (rwx) : ORIGIN = 0x08003000, LENGTH = 4K
  ram  (rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

INCLUDE libopencm3_stm32f0.ld


SECTIONS {
    user_data : {
        . = ALIGN(4);
        *(.user_data)
        . = ALIGN(4);
    } > data
}
