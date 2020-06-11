FROM gcc AS builder
COPY *.[ch] /tmp/src/
RUN mkdir /tmp/bin \
  && gcc -static -o /tmp/bin/colander /tmp/src/*.c \
  && chmod 500 /tmp/bin/colander

FROM scratch
COPY docker/passwd docker/group /etc/
USER colander:colander
COPY --from=builder --chown=colander:colander /tmp/bin/colander /usr/bin/colander
ENTRYPOINT [ "/usr/bin/colander" ]