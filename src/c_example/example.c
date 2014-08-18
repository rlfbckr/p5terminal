#include "processing-terminal.h"
/*
    Copyright (c) 2014 by Ralf Baecker <rlfbckr ~AT~ gmail.com>

    This file is part of Processing-Terminal.

    A wrapper around libcaca and ncurses to offer an ascii-art version to
    the processing language

    Processing-Terminal is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Processing-Terminal.  If not, see
    <http://www.gnu.org/licenses/>.
*/

processingterminal pt;
int width;
int height;

void setup() {
    size(80,20);
    printf("hello setup\n");

}
void draw() {
	background(255,0,0);
    stroke(255);
    line(0, 0, width, height);
}
int main(int argc, char **argv) {
    if (init()) {
        return 0;
    }
    processing_terminal();
    return 0;
}

