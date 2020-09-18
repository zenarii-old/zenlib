// TODO(Abi): sort this + message box out
internal void
LinuxError(const char * Title, const char * Message) {
    fprintf(stderr, "%s: %s\n", Title, Message);
}