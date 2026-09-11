///
/// Image
///

interface Props {
  image: string;
}

const Image = ({ image }: Props) => {
  const src = new URL(`../images/${image}`, import.meta.url).href;

  return (
    <div style={{ display: "flex", justifyContent: "center", alignItems: "center", width: "100%", padding: "16px" }}>
      <div 
        className="splash-image transparent" 
        style={{
          borderRadius: "12px",
          overflow: "hidden",
          boxShadow: "0 8px 24px rgba(0,0,0,0.3)",
          border: "1px solid rgba(255,255,255,0.1)",
          maxWidth: "100%",
          maxHeight: "80vh"
        }}
      >
        <img 
          src={src} 
          alt="Macondian View"
          style={{ width: "100%", height: "auto", display: "block", objectFit: "cover" }}
        />
      </div>
    </div>
  );
};

export default Image;