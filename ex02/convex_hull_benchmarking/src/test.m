mkdir pics
for i = [0:4]
	inputName = sprintf ("build/output-%d.txt", i);
	imageName = sprintf ("pics/pic-%d.png", i)
	data = dlmread (inputName)

	handle = figure('visible', 'off');
	plot(data(1,:), data(2:end,:), '--s', 'linewidth', 2, 'markersize', 5)
	legend("toussaint", "bykat", "eddy", "graham-andrew", "jarvis");
	title(sprintf('Convex Hull. Sample points on hull boundary: %.1f%% sample points within hull: %.1f%%', i/5 * 100 , (5-i)/5 * 100));
	xlabel('number of points');
	ylabel('average time (ms)');
	print(imageName);
	close(handle);
end;
