// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

namespace mac_wnd
{
void init();
bool init_window(const char *title, int width, int height);
void destroy_window();

void get_display_size(int &width, int &height);
void get_video_modes_list(Tab<String> &list);
void *get_main_window();
void *get_main_view();
} // namespace mac_wnd
