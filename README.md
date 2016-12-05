Prepare:

`sudo apt-get install gnuplot-x11`

Build:

`make [DEBUG=y]`

Run:

`./bin/nn <input_file_path> <coord_file_path>`

For example:

`./bin/nn input/5.txt input/coord.txt`

Show output plot:

`gnuplot plot/plot.pl`

Show weights plot:

`gnuplot plot/plot_weights.pl`
