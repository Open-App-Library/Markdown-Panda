/*
 * Various plugins that can be run after HTML->Markdown conversion.
 * Note: These are currently hard-coded into MarkdownPanda and the
 *       user can't utilize these.
 */

#ifndef PLUGINS_H
#define PLUGINS_H

/*
 * Ensures there is a newline at end of string.
 */
char *plugin_ensure_newline(char *str);

/*
 * Will convert this...
 * | Name | Age |
 * | -- | -- |
 * | Joe | 19 |
 *
 * ...to this!
 * | Name | Age |
 * | ---- | --- |
 * | Joe  | 19  |
 */
char *plugin_beautify_tables(char *str);

#endif
