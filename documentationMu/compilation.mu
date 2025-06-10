<00>
 <e>*** <r>Compilation</r> ***</e>

 <c>parser_exit0_mu</c>
 gcc -I. -o parser_exit0_mu parser_exit0_mu.c utils.c tagTermiosMu/tag_termios_mu.c

 <c>parser_scroll_mu</c>
 gcc parser_scroll_mu.c utils.c -I./tagTermiosMu -L./tagTermiosMu -ltag_termios_mu -o parser_scroll_mu -Wall

 <c>parser_flipp_mu</c>
 gcc -g -o parser_flipp_mu parser_flipp_mu.c utils.c -I. -LtagTermiosMu -ltag_termios_mu -std=c99

 <c>parser_navigator_mu.c</c>
 gcc -g -o parser_navigator_mu parser_navigator_mu.c utils.c -I. -LtagTermiosMu -ltag_termios_mu -std=c99
<01>
