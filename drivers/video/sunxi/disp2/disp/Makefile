obj-m += disp.o

disp-objs :=  dev_disp.o dev_fb.o
disp-objs	+= disp_sys_intf.o
disp-objs	+= de/disp_display.o de/disp_features.o de/disp_device.o de/disp_lcd.o de/disp_manager.o de/disp_private.o \
							de/disp_smart_backlight.o
disp-objs += de/lowlevel_sun8iw6/de_fcc.o de/lowlevel_sun8iw6/de_gsu.o de/lowlevel_sun8iw6/de_hal.o de/lowlevel_sun8iw6/disp_al.o \
						de/lowlevel_sun8iw6/de_peak.o de/lowlevel_sun8iw6/de_rtmx.o de/lowlevel_sun8iw6/de_scaler_table.o\
						de/lowlevel_sun8iw6/de_vep_table.o de/lowlevel_sun8iw6/de_vsu.o de/lowlevel_sun8iw6/de_feat.o \
						de/lowlevel_sun8iw6/de_lcd.o
disp-objs += lcd/panels.o lcd/lcd_source.o lcd/default_panel.o


