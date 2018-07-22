#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

FBTCD=${FBTCD:-$SRCDIR/FBTCd}
FBTCCLI=${FBTCCLI:-$SRCDIR/FBTC-cli}
FBTCTX=${FBTCTX:-$SRCDIR/FBTC-tx}
FBTCQT=${FBTCQT:-$SRCDIR/qt/FBTC-qt}

[ ! -x $FBTCD ] && echo "$FBTCD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
FBTCVER=($($FBTCCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for bitcoind if --version-string is not set,
# but has different outcomes for bitcoin-qt and bitcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$FBTCD --version | sed -n '1!p' >> footer.h2m

for cmd in $FBTCD $FBTCCLI $FBTCTX $FBTCQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${FBTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${FBTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m