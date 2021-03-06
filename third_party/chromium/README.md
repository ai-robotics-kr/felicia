### Specification

- Name: chromium
- URL: [https://github.com/chromium/chromium.git](https://github.com/chromium/chromium.git)
- Commit: 5db095c2653f332334d56ad739ae5fe1053308b1
- License: BSD 3-Clause
- License File: LICENSE

### Description

Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web.

### Note

#### base
Followings were taken or changed to original code.
- Support to build using bazel.
  - TODO: add configurable settings, currently configurable value is fixed.
  - TODO: adopt compiler flags from build/.
- Modify codes to be compiled.
- Not compiled
  - base64url*, base64*
  - i18n/*
  - trace_event/trace_event_etw_export_win*
  - Maybe these lists can be more.
- See difference in base/base.diff

#### build
- Take minimal to be compiled with bazel.

#### net
Followings were taken or changed to original code.
- Support to build using bazel.
- Modify codes to be compiled.
- Take minimal ConnectionType from network_changes_notifier.
- Remove NetLogWithSource from socket.
- Remove DscpManager from udp_socket_win.
- See difference in net/net.diff

#### url
- Take minimal to support ip parsing.
- See difference in url/url.diff