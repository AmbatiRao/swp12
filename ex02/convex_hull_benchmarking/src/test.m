mkdir pics
for i = [0:4]
	inputName = sprintf ("build/output-%d.txt", i);
	imageName = sprintf ("pics/pic-%d.png", i)
	data = dlmread (inputName)

	handle = figure('visible', 'off');
	plot(data(1,:), data(2:end,:), 'linewidth', 2)
	legend("toussaint", "bykat", "eddy", "graham-andrew", "jarvis");
	print(imageName);
	close(handle);
end;
