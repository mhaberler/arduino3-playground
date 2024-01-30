#include "Sensor.hpp"
#include "ArduinoJson.h"
#include "TreeWalker.hpp"
#include "fsVisitor.hpp"

static bool _fs_visit(fs::FS &fs, Stream &out, fs::File &f, uint32_t flags) {
    bool result = false;
    time_t t = f.getLastWrite();
    struct tm *tm = localtime(&t);
    bool isDir = f.isDirectory();
    if (flags & VA_PRINT) {
        out.printf("%s %u %s\n", isDir ? "d" : "-", f.size(),
                   f.path());
    }
    if (isDir & (flags & VA_RECURSIVE)) {
        return true;
    }
    if (!isDir) { // regular file
        if (flags & VA_LOAD_CONFIG) {
            if (flags & VA_DEBUG) {
                out.printf("loading %s size %u\n", f.path(), f.size());
            }
            equipment.addUnit(f.path());
        }
        if (flags & VA_ERASE_FILES) {
            bool rc = fs.remove(f.path());
            if (flags & VA_DEBUG) {
                out.printf("erasing %s - result: %d\n", f.path(), rc);
            }
        }
        return true; // continue
    }
    return false;
}

void fsVisitor(fs::FS &fs, Stream &fmt, const char *topdir, uint32_t flags,
               uint8_t levels) {
    const Visitor v = makeFunctor((Visitor *)NULL, _fs_visit);
    TreeWalker walker(v);
    walker.begin(fs, fmt, flags, topdir, levels);
}
