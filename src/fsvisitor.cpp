//#ifdef FSVISITOR
// #include "SdFat.h"
// #include "common/FsDateTime.h"

// #include <Fmt.h>
#include <fmt/chrono.h>

// #include "Cache.hpp"
#include "TreeWalker.hpp"
// #include "common/FsStructs.h"
#include "fsVisitor.hpp"
// #include "logmacros.hpp"

// Cache psramCache;

static bool _fs_visit(fs::FS &fs, Stream &out, fs::File &f, uint32_t flags) {
  bool result = false;
  time_t t = f.getLastWrite();
  struct tm *tm = localtime(&t);
  bool isDir = f.isDirectory();
  if (flags & VA_PRINT) {
            log_e("%s %u %s", isDir ? "d" : "f", f.size(),
              f.path());

    // LOGD("{} {:>7} {:<50} {:%Y-%m-%d %H:%M:%S}", isDir ? "d" : "f", f.size(),
    //      f.path(), *tm);
  }
  if (isDir) {
    return true; // recurse downwards
  }
  // if (flags & VA_CACHE) {
  //   result = psramCache.insert(fs, f.path());
  //   if (!result && (flags & VA_DEBUG)) {
  //     LOGD("toPsram() failed, stopping");
  //   }
  // }
  return true; // continue
}

// static bool _sdfat_visit(SdFat &fs, Stream &out, SdBaseFile &f, uint32_t flags) {
//   bool result = false;
//   Stream line;
//   // Print *pr = &line.stream();
//   Print *pr = &Serial;
  
//   bool isDir = f.isDirectory();
//   if (flags & VA_PRINT) {
//     f.ls(pr, LS_DATE | LS_SIZE); // LS_R - Recursive list of subdirectories.
//   }
//   // while (line.available()) {
//   //   Serial.write((char)line.read());
//   // }

//   if (isDir) {
//     return true;
//   }
//   // if (flags & VA_CACHE) {
//   //   result = psramCache.insert(fs, f.path());
//   //   if (!result && (flags & VA_DEBUG)) {
//   //     LOGD("toPsram() failed, stopping");
//   //   }
//   // }
//   return result;
// }

// void fsVisitor(SdFat &fs, Stream &fmt, const char *topdir, uint32_t flags,
//                uint8_t levels) {

//   uint32_t pre = ESP.getFreePsram();
//   if (flags & VA_CACHE) {
//     LOGD("pre: free PSRAM={}", pre);
//   }

//   const SdFatVisitor v = makeFunctor((SdFatVisitor *)NULL, _sdfat_visit);
//   TreeWalker walker(v);

//   uint32_t now = millis();

//   walker.begin(fs, fmt, flags, topdir, levels);

//   uint32_t mS = millis() - now;
//   uint32_t post = ESP.getFreePsram();

//   uint32_t total_bytes, total_files;
//   psramCache.stats(total_bytes, total_files);

//   if (flags & VA_CACHE) {
//     LOGD("post: free PSRAM={} used={} - {:.1f}%", post, pre - post,
//          float(pre - post) * 100.0 / pre);
//   }
//   LOGD("{} files, {} bytes cached in {:.3f} S - {:.3f} kB/s", total_files,
//        total_bytes, mS / 1000.0, (total_bytes / 1024.0) / (mS / 1000.0));
// }

void fsVisitor(fs::FS &fs, Stream &fmt, const char *topdir, uint32_t flags,
               uint8_t levels) {

  uint32_t pre = ESP.getFreePsram();
  // LOGD("pre: free PSRAM={}", pre);

  const Visitor v = makeFunctor((Visitor *)NULL, _fs_visit);
  TreeWalker walker(v);

  uint32_t now = millis();

  walker.begin(fs, fmt, flags, topdir, levels);

  // uint32_t mS = millis() - now;
  // uint32_t post = ESP.getFreePsram();

  // uint32_t total_bytes, total_files;
  // psramCache.stats(total_bytes, total_files);

  // LOGD("post: free PSRAM={} used={} - {:.1f}%", post, pre - post,
  //      float(pre - post) * 100.0 / pre);
  // LOGD("{} files, {} bytes cached in {:.3f} S - {:.3f} kB/s", total_files,
  //      total_bytes, mS / 1000.0, (total_bytes / 1024.0) / (mS / 1000.0));
}

// #endif