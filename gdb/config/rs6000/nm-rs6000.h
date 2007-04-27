/* IBM RS/6000 native-dependent macros for GDB, the GNU debugger.
   Copyright 1986, 1987, 1989, 1991, 1992, 1994, 1996, 1999, 2000, 2001, 2007
   Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* When a child process is just starting, we sneak in and relocate
   the symbol table (and other stuff) after the dynamic linker has
   figured out where they go.  */

#define	SOLIB_CREATE_INFERIOR_HOOK(PID)	\
  do {					\
    xcoff_relocate_symtab (PID);	\
  } while (0)

/* When a target process or core-file has been attached, we sneak in
   and figure out where the shared libraries have got to.  */

#define	SOLIB_ADD(a, b, c, d)	\
  if (PIDGET (inferior_ptid))	\
    /* Attach to process.  */  \
    xcoff_relocate_symtab (PIDGET (inferior_ptid)); \
  else		\
    /* Core file.  */ \
    xcoff_relocate_core (c);

extern void xcoff_relocate_symtab (unsigned int);
struct target_ops;
extern void xcoff_relocate_core (struct target_ops *);

/* If ADDR lies in a shared library, return its name.  */

#define	PC_SOLIB(PC)	xcoff_solib_address(PC)
extern char *xcoff_solib_address (CORE_ADDR);

/* Flag for machine-specific stuff in shared files.  FIXME */
#define DEPRECATED_IBM6000_TARGET

/* AIX has a couple of strange returns from wait().  */

#define CHILD_SPECIAL_WAITSTATUS(ourstatus, hoststatus) ( \
  /* "stop after load" status.  */ \
  (hoststatus) == 0x57c ? (ourstatus)->kind = TARGET_WAITKIND_LOADED, 1 : \
  \
  /* signal 0. I have no idea why wait(2) returns with this status word.  */ \
  /* It looks harmless. */ \
  (hoststatus) == 0x7f ? (ourstatus)->kind = TARGET_WAITKIND_SPURIOUS, 1 : \
  \
  /* A normal waitstatus.  Let the usual macros deal with it.  */ \
  0)

