const React = require("react");
if (typeof window === 'undefined') {
	module.exports = () => (
		<div>Please enable JavaScript to use the analyser.</div>
	);
} else {
	module.exports = require("components/analyser/AnalyserView");
}
