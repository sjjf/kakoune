# Attempt to detect the windowing environment we're operating in
#
# Supported environments and their defaults are:
#  * X11 - each new window is a new top level window
#  * tmux - new windows are vertical tmux panes
#  * screen - new windows are vertical screen regions
#  * iterm - new windows are vertical iterm panes
#  * kitty - new windows are kitty windows
#
# Each module must define two aliases:
#  * terminal - create a new terminal with sensible defaults
#  * focus - focus the specified client, defaulting to the current client
#
# Modules may also implement the following repl commands:
#  * repl - create a new window for repl interaction
#  * send-text - send the selected text to the repl window

hook -group windowing global KakBegin .* %sh{
    # detect tmux and screen
    if [ -n "$TMUX" ]; then
        echo "require-module tmux"
    elif [ -n "$STY" ]; then
        echo "require-module screen"
    elif [ "$TERM" = "xterm-kitty" ]; then
        # kitty requires $TERM set to xterm-kitty
        echo "require-module kitty"
    elif [ "$TERM_PROGRAM" = "iTerm.app" ]; then
        echo "require-module iterm"
    elif [ -n "$DISPLAY" ]; then
        echo "require-module x11"
    fi
}
