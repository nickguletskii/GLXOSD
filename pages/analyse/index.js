const React = require("react");
if (process.env.STATIC_ENV) {
	module.exports = () => (
		<div>Please enable JavaScript to use the analyser.</div>
	);
} else {
	module.exports = require("components/analyser/AnalyserView");
}
